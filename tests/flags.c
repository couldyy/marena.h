#define MARENA_STATIC_RETURN_NULL_ON_FULL
#define MARENA_IMPLEMENTATION
#include <stdio.h>
#include <stdbool.h>
#include "../marena.h"

int allocate(Arena* a)
{
    int allocated = 0;
    for (size_t alloc_size = 64; allocated < a->page_size * 1.5;) {
        if ((arena_alloc(a, alloc_size)) == NULL)
            return -1;
        allocated += alloc_size;
    }
    return allocated;
}

int get_n_allocs(size_t page_size, size_t alloc_size)
{
    int allocs_per_miss = page_size/alloc_size;
    if (allocs_per_miss * alloc_size <= page_size)
        allocs_per_miss += 1;
    
    if (1.0f/(long double)allocs_per_miss > MARENA_PAGEMISS_FACTOR)
        return allocs_per_miss;
    else 
        return -1;
}

const size_t alloc_size = 64;

int main()
{
    printf("Arena dynamic page size\n");
    Arena arena_dynamic_page_size = {0};

    size_t init_page_size = 2560;
    arena_dynamic_page_size.page_size = init_page_size;
    arena_dynamic_page_size.flags = MA_O_ARENA_DYNAMIC | MA_O_DYNAMIC_PAGE_SIZE;

    int n_allocs = get_n_allocs(arena_dynamic_page_size.page_size, alloc_size);
    if (n_allocs <= 0) {
        fprintf(stderr, "No page growth for that factor(%f), page_size(%zu) and alloc_size(%zu)\n", 
            MARENA_PAGEMISS_FACTOR, arena_dynamic_page_size.page_size, alloc_size);
        exit(1);
    }

    for (int i = 0; i < n_allocs; i++) {
        arena_alloc(&arena_dynamic_page_size, alloc_size);
    }
    printf("init_page_size < current_page_size ? %s\n", init_page_size < arena_dynamic_page_size.page_size ? "true" : "false");
    printf("------------------------------\n");

    printf("Arena dynamic\n");
    Arena arena_dynamic = {0};
    arena_dynamic.page_size = 256;
    arena_dynamic.flags = MA_O_ARENA_DYNAMIC;

    int allocated = allocate(&arena_dynamic);
    //printf("allocated: %zu, page_size: %zu\n", allocated, arena_dynamic.page_size);
    void* next_page = arena_dynamic.end;
    printf("last_page != start_page ? %s\n", next_page != arena_dynamic.start ? "true" : "false");

    arena_reset(&arena_dynamic);

    allocated = allocate(&arena_dynamic);
    printf("arena_reset()\n");
    //printf("allocated: %zu, page_size: %zu\n", allocated, arena_dynamic.page_size);
    printf("arena.end == last_page(previous) ? %s\n", next_page == arena_dynamic.end ? "true" : "false");
    printf("------------------------------\n");

    printf("Arena static\n");

    Arena arena_static = {0};
    arena_static.page_size = 256;
    arena_static.flags = MA_O_ARENA_STATIC;
    
    allocated = allocate(&arena_static);
    if (allocated < 0) {
        printf("Arena static is full\n");
    }

    return 0;
}
