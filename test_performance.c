#include <stdio.h>
#include <time.h>

#define MARENA_IMPLEMENTATION
#include "marena.h"


#define MAX_PAGE_SIZE (1024 * 1024 * 100) // 10 MiB
#define MICRO_TO_MILI ((long double)1000.0)

#define N 1000000UL

// allocation size will grow at each alloc
void test_dynamic(size_t page_size)
{
    clock_t start = clock();
    Arena a = {.page_size = page_size};
    //Arena a = {.page_size = 8192};
    size_t alloc_size = 64;
    for (size_t i = 0; i < N; i++) {
        continue; 
    }
}

void test_static(size_t page_size, size_t alloc_size)
{
        clock_t start = clock();
        Arena a = {.page_size = page_size};
        //Arena a = {.page_size = 8196};

        for (size_t allocs_count = 0; allocs_count < N; allocs_count++) {
            //arena_alloc(&a, alloc_size);
            malloc(alloc_size);
            //total_allocated += alloc_size;
        }
        arena_free(&a);
        clock_t end = clock();
        //printf("N allocations: %llu, page size: %zu, alloc_size: %zu (total allocated: %zu)  === %llf ms\n", N, page_size, alloc_size, total_allocated, (long double)(end - start) / MICRO_TO_MILI);
        printf("N allocations: %llu, page size: %zu, alloc_size: %zu  === %llf ms\n", N, page_size, alloc_size, (long double)(end - start) / MICRO_TO_MILI);
}
int main()
{
    size_t alloc_size;
    for (size_t page_size = 256; page_size <= MAX_PAGE_SIZE; page_size *= 2) {
        alloc_size = 64;
        test_static(page_size, alloc_size);
        //alloc_size = page_size / 4;
        //test_static(page_size, alloc_size);
        printf("----------------------------------------\n"); 
    }
    return 0;
}
