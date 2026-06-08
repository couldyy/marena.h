#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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
    (void)arena_flags;
    clock_t start, end;

    start = clock();
    Arena a = {0};
    a.page_size = page_size;
    //a.flags = arena_flags;

    uint64_t total_allocated = 0;
    uint64_t total_allocated_aligned = 0;

    //Arena a = {.page_size = 8192};
    for (size_t i = 0; i < arr_size; i++) {
        size_t alloc_size = alloc_sizes_arr[i];
        arena_alloc(&a, alloc_size);
        //arena_alloc_zero(&a, alloc_size);

        total_allocated += alloc_size;
        total_allocated_aligned += MA_ALIGN(alloc_size);
    }
    arena_free(&a);
    end = clock();
    printf("N allocs: %zu, page_size: %zu, total: %f Mb(aligned: %f Mb), avg_alloc_size: %f, ===>   %Lf ms\n",
        N, page_size, total_allocated/1024.0/1024.0, total_allocated_aligned/1024.0/1024.0, (double)total_allocated/(double)N,
        (long double)(end - start) / MICRO_TO_MILI);
}

int main()
{
    size_t *alloc_sizes = malloc(N * sizeof(size_t));

    int seed = 417446206;
    srand(seed);
    printf("\n\n->> DYNAMIC SIZE ALLOCAION TEST <<-\n");
    for (size_t alloc_avg = 64; alloc_avg <= 1000; alloc_avg += 50) {
        printf("\n========= AVG alloc (%zu)=================\n", alloc_avg);
        for (size_t page_size = 256; page_size <= MAX_PAGE_SIZE; page_size *= 2) {
            for (size_t j = 0; j < N; j++) {
                alloc_sizes[j] = get_dynamic_alloc_size(alloc_avg);
            }
            test(page_size, 0, alloc_sizes, N);
        }
        //test_static(page_size, alloc_size);
    }

    size_t alloc_size = 64;
    for (size_t i = 0; i < N; i++) {
        alloc_sizes[i] = alloc_size; 
    }
    printf("\n\n->> STATIC SIZE ALLOCAION TEST <<-\n");
    for (size_t page_size = 256; page_size <= MAX_PAGE_SIZE; page_size *= 2) {
        test(page_size, 0, alloc_sizes, N);
        printf("----------------------------------------\n"); 
    }
    return 0;
}
