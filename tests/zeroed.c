#include <stdio.h>
#include <stdlib.h>
#define MARENA_IMPLEMENTATION
//#define MARENA_DEBUG
#include "../marena.h" 

#define N 10
void fill_with_garbage(Arena* arena, size_t n)
{
    char* addr = arena_alloc(arena, n);
    for (size_t i = 0; i < n; i++) {
        addr[i] = 0xdc; // So not to collide with init value in MARENA_DEBUG (which is 0xcd)
    }
}

int main()
{
    //Arena* arena = arena_init(16);
    Arena arena = { .page_size = 64 };
    //Arena arena = { .page_size = 512, .flags = 192};
    
    fill_with_garbage(&arena, sizeof(int) * N * 100);
    arena_reset(&arena);
    
    size_t total_bytes = 0;
    printf("Mem after alloc_zero():\n");
    int* arr_zeroed = arena_alloc_zero(&arena, (sizeof(int)+1) * N);
    for(int i = 0; i < N; i++) {
        printf("[%d]: %d\n", i, arr_zeroed[i]);
    }
    total_bytes += (sizeof(int)+1) * N;
    printf("------------------------------\n");

    printf("Mem after alloc():\n");
    int* arr = arena_alloc(&arena, (sizeof(int)) * N);
    for(int i = 0; i < N; i++) {
        printf("[%d]: %d\n", i, arr[i]);
    }
    total_bytes += (sizeof(int)) * N;
    printf("------------------------------\n");

    for(int i = 0; i < N; i++) {
        arr_zeroed[i] = N - i;
        arr[i] = N - i;
    }

    printf("alloc_zeroed() after filling with data:\n");
    for(int i = 0; i < N; i++) {
        printf("[%d]: %d\n", i, arr_zeroed[i]);
    }
    printf("------------------------------\n");

    printf("alloc() after filling with data:\n");
    for(int i = 0; i < N; i++) {
        printf("[%d]: %d\n", i, arr[i]);
    }
    printf("------------------------------\n");

    //arena_free(&arena);
    printf("Mem after arena_reset_zeroed():\n");
    arena_reset_zero(&arena);
    for(int i = 0; i < total_bytes; i++) {
        printf("[%d]: %d\n", i, arr[i]);
    }
    printf("------------------------------\n");

    printf("Memory after alloc():\n");
    arr = arena_alloc(&arena, N * 10);
    for(int i = 0; i < N*10; i++) {
        printf("[%d]: %d\n", i, arr[i]);
    }
    printf("------------------------------\n");
    fflush(stdout);
    return 0;
}
