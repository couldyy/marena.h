#include <stdio.h>
#include <stdlib.h>
#define MARENA_IMPLEMENTATION
#include "marena.h" 

#define N 10

int main()
{
    //Arena* arena = arena_init(16);
    Arena arena = { .page_size = 64 };
    //Arena arena = { .page_size = 512, .flags = 192};
    
    int* arr = arena_alloc(&arena, (sizeof(int)+1) * N);
    for(int i = 0; i < N; i++) {
        arr[i] = i;
    }
    for(int i = 0; i < N; i++) {
        printf("[%d]: %d\n", i, arr[i]);
    }
    printf("------------------------------\n");
    arr = arena_alloc(&arena, N* 10);
    arr = arena_alloc(&arena, N * 11);
    for(int i = 0; i < N; i++) {
        arr[i] = N - i;
    }
    for(int i = 0; i < N; i++) {
        printf("[%d]: %d\n", i, arr[i]);
    }
    printf("------------------------------\n");

    //arena_free(&arena);
    arena_reset(&arena);
    for(int i = 0; i < N; i++) {
        printf("[%d]: %d\n", i, arr[i]);
    }
    printf("------------------------------\n");

    arr = arena_alloc(&arena, N * 10);
    for(int i = 0; i < N; i++) {
        arr[i] = i;
    }
    for(int i = 0; i < N; i++) {
        printf("[%d]: %d\n", i, arr[i]);
    }
    printf("------------------------------\n");
    return 0;
}
