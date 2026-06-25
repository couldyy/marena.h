#include <stdio.h>
#include <stdlib.h>
#define MARENA_IMPLEMENTATION
#include "../marena.h" 

#define N 10

int main()
{
    Arena arena = { .page_size = 64 };
        
    int* arr = arena_alloc(&arena, sizeof(int) * N);
    for(int i = 0; i < N; i++) {
        arr[i] = i;
    }
    for(int i = 0; i < N; i++) {
        printf("[%d]: %d\n", i, arr[i]);
    }
    printf("------------------------------\n");
 
    arena_reset(&arena);
	// memory is still available, but DONT do that
    for(int i = 0; i < N; i++) {
        printf("[%d]: %d\n", i, arr[i]);
    }
    printf("------------------------------\n");

    arr = arena_alloc(&arena, sizeof(int) * N);
    for(int i = 0; i < N; i++) {
        arr[i] = N - i;
    }
    for(int i = 0; i < N; i++) {
        printf("[%d]: %d\n", i, arr[i]);
    }
    printf("------------------------------\n");
    arena_free(&arena);
    return 0;
}
