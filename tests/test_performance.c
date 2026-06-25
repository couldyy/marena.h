#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <strings.h>
#include <time.h>
#include <math.h>

#define MARENA_IMPLEMENTATION
#include "../marena.h"


#define MAX_PAGE_SIZE (1024 * 1024 * 100) // 10 MiB
#define MICRO_TO_MILI ((long double)1000.0)

#define N 1000000UL

// average for exp random distribution, all allocs will be around that size
#define DYNAMIC_ALLOC_AVG 192

long double norm(long double start, long double end, long double value) 
{
    return (value - start)/(end - start);
}
long double randp() {
    return norm(0, RAND_MAX, rand());
}

size_t get_static_alloc_size(size_t return_arg) 
{
    return return_arg;
}

size_t get_dynamic_alloc_size(double avg)
{
    //long double k_intensity = 1.0/512.0;  // intensity
    //long double k_intensity = 1.0/1024.0;  // intensity
    //long double k_intensity = 1.0/192.0;  // intensity
    long double k_intensity = 1.0/avg;  // intensity
    long double uniform_rand = randp();
    long double size = log(-uniform_rand + 1) / -k_intensity;
    return (size_t)size;
}

// measures Arena initialization, allocations, arena_free combined
void test(size_t page_size, int arena_flags, size_t* alloc_sizes_arr, size_t arr_size)
{
    clock_t start, end;

    start = clock();
    Arena a = {0};
    a.page_size = page_size;
    a.flags = arena_flags;

    uint64_t total_allocated = 0;
    uint64_t total_allocated_aligned = 0;

    //Arena a = {.page_size = 8192};
    for (size_t i = 0; i < arr_size; i++) {
        size_t alloc_size = alloc_sizes_arr[i];
        //arena_alloc(&a, alloc_size);
        arena_alloc_zero(&a, alloc_size);

        total_allocated += alloc_size;
        total_allocated_aligned += MA_ALIGN(alloc_size);
    }
    arena_free(&a);
    end = clock();
    printf("N allocs: %zu, page_size: %zu, total: %f Mb(aligned: %f Mb), avg_alloc_size: %f, ===>   %Lf ms\n",
        N, page_size, total_allocated/1024.0/1024.0, total_allocated_aligned/1024.0/1024.0, (double)total_allocated/(double)N,
        (long double)(end - start) / MICRO_TO_MILI);
}


typedef enum {
    Static, 
    Dynamic,
} Test;

typedef struct {
    Test test;
    int arena_flags;
} Mode;

void print_usage(char* program_name)
{
    printf("%s -t <mode> [-f <arena_flags_list>]\n"
    "%s --test <mode> [--flags <arena_flags_list>]\n"
    "\t -t, --test <mode>, the <mode> is:\n"
    "\t\tstatic - testing on a fixed allocation size over different page_size\n"
    "\t\tdynamic - testing on a randomly generated allocation size based on exp distribution\n"

    "\t -f, --flags <arena_flags_list>, coma separated list of the following:\n"
    "\t\tarena_static - size of arena is fixed on initialization\n"
    "\t\tarena_dynamic - arena grows as it gets out of memory\n"
    "\t\tdynamic_page_size - page_size grows if page allocation happens so frequently (this and 'arena_static' are mutually exclusive\n", program_name, program_name);
}
Mode parse_flags(int argc, char** argv)
{
    Mode mode = {0};
    if (argc < 3) {
        fprintf(stderr, "Invalid usage\n");
        print_usage(argv[0]);
        exit(1);
    }
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-t") || !strcmp(argv[i], "--test")) {
            if (i + 1 < argc) {
                if (!strcmp(argv[i+1], "static")) mode.test = Static;
                else if (!strcmp(argv[i+1], "dynamic")) mode.test = Dynamic;
                else {
                    goto err_test;
                }
                i++;
            }
            else {
            err_test:
                fprintf(stderr, "Invalid usage, <mode> unspecified. -t or --test <mode>\n");
                exit(1);
            }
        }
        if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "--flags")) {
            if (i + 1 < argc) {
                char* flags_list = argv[i+1];
                char* flag = NULL;
                while ((flag = strtok(flags_list, ","))) {
                    flags_list = NULL;
                    if (!strcmp(flag, "arena_static")) mode.arena_flags |= MA_O_ARENA_STATIC;
                    else if (!strcmp(flag, "arena_dynamic")) mode.arena_flags |= MA_O_ARENA_DYNAMIC;
                    else if (!strcmp(flag, "dynamic_page_size")) mode.arena_flags |= MA_O_DYNAMIC_PAGE_SIZE;
                    else {
                        fprintf(stderr, "Invalid flag '%s'\n", flag);
                        //print_usage(argv[0]);
                        exit(1);
                    }
                }
            }
            else {
                fprintf(stderr, "Invalid usage! <arena_flags_list> unspecified\n"); 
                print_usage(argv[0]);
            }
        }
    }
    return mode;
}

int main(int argc, char** argv)
{
    Mode mode = parse_flags(argc, argv);
    printf("mode: %d, flags: %d\n", mode.test, mode.arena_flags);

    size_t *alloc_sizes = malloc(N * sizeof(size_t));

    int seed = 417446206;
    srand(seed);
    if (mode.test == Dynamic) {
        printf("\n\n->> DYNAMIC SIZE ALLOCAION TEST <<-\n");
        for (size_t alloc_avg = 64; alloc_avg <= 1000; alloc_avg += 50) {
            printf("\n========= AVG alloc (%zu)=================\n", alloc_avg);
            for (size_t page_size = 256; page_size <= MAX_PAGE_SIZE; page_size *= 2) {
                for (size_t j = 0; j < N; j++) {
                    alloc_sizes[j] = get_dynamic_alloc_size(alloc_avg);
                }
                test(page_size, mode.arena_flags, alloc_sizes, N);
            }
            //test_static(page_size, alloc_size);
        }
    }
    else if (mode.test == Static) {
        size_t alloc_size = 64;
        for (size_t i = 0; i < N; i++) {
            alloc_sizes[i] = alloc_size; 
        }
        printf("\n\n->> STATIC SIZE ALLOCAION TEST <<-\n");
        for (size_t page_size = 256; page_size <= MAX_PAGE_SIZE; page_size *= 2) {
            test(page_size, mode.arena_flags, alloc_sizes, N);
            printf("----------------------------------------\n"); 
        }
    }
    return 0;
}
