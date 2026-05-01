#ifndef MARENA_H
#define MARENA_H

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

/*
 TODO:
    [x] store pointer to end in Arena struct and DONT iterate over Pages, just allocate at last one
    [x] create functions for resetting arena (mark all pages as free)
    [ ] create context_* funcions if there are corresponding #define (context_alloc(), context_reset(), context_free())
        which will allow to call all functions without providing Arena address
    [ ] implement different backends (single macro and different unerlying functions for it)
    [ ] store and restore functions for arena memory
    [ ] handle arena_alloc(&a, 0) (size = 0)
    [ ] implement with some debug compile flage, some debug info:
        - how many allocations were called on arena
        - how mant bytes allocated (in arena and Page)
        - how many times new page was allocated
        - how many bytes lost (fragmentation: (mb calculate avg lost per allocation)
                    Page size: 8192 [******|               ] -> [******************************|   ]
                            1000 bytes ^^                               ^^^^^
                                            7500 requested - dont fit, new page has to be allocated, 7192 bytes lost

        - how many times allocation exceed 'page_size'
    [ ]? create algorithm, so that if allocation frequently causes new page allocation - increase page size?

    [ ]? Rename Page to Region?
 */

#define MA_DEFAULT_PAGE_SIZE 8196

// should be enough for most systems. You can manually redefine it, if this alignment doesnt work for you
#define MA_ALIGNMENT (sizeof(void*))

#define MA_ALIGN(sz) ((sz + MA_ALIGNMENT - 1) & ~(MA_ALIGNMENT - 1))


// pointer to that struct is actual start of page, usable memory is at &page + sizeof(Page)
//  'free' contains start address of free memory in that page
typedef struct Page {
    void* start_free;   // pointer (offset from the begining) to the start of allocatable memory in page
    size_t capacity;    // bytes
    size_t size;        // bytes

    struct Page* next;
} Page;

typedef struct {
    Page* start;    // TODO: some pages mey be full, in order not to iterate through them, maybe store some pointer to page that has free memory?
    Page* end;    
    size_t page_size;       // size for further allocations of page
    //uint8_t flags;
} Arena;



// initializes page
Page* init_page(size_t size);

// initializes arena on the heap
Arena* arena_init_heap(size_t size);

// allocates at least 'size' aligned bytes within 'arena' memory
void* arena_alloc(Arena* arena, size_t size);

// Deallocates all pages in arena
// Note: does NOT deallocate 'arena' struct itself
void arena_free(Arena* arena);

// Marks all pages as free
void arena_reset(Arena* arena);

#endif // MARENA_H

#ifdef MARENA_IMPLEMENTATION
// allocates requested size + sizeof(Page) strcture, and writes that structure in the beginnings
Page* init_page(size_t size)
{
    assert(size > 0);
    Page* page = malloc(sizeof(Page) + size);
    assert(page != NULL && "malloc failed at init page");
    void* usable_mem_start = (char*)page + sizeof(Page);  // offset actual start of usable memory TODO: what if user does smth nasty with metadata?
    page->start_free = usable_mem_start;
    page->capacity = 0;
    page->size = size;
    page->next = NULL;
    return page;
}

Arena* arena_init_heap(size_t size)
{
    assert(size > 0);
    Arena* arena = malloc(sizeof(Arena));
    assert(arena != NULL && "Arena init failed");
    arena->start = init_page(size);
    arena->end = arena->start;
    arena->page_size = size;
    return arena; 
}

void* arena_alloc(Arena* arena, size_t size)
{
    assert(arena != NULL);
    size_t aligned_size = MA_ALIGN(size);
    Page* page = arena->end;
    Page* prev_page = NULL;
    while (page != NULL && page->capacity + aligned_size > page->size) {
        prev_page = page;
        page = page->next;
    }
    // just allocate new page, if there are no free space left in pages
    if (page == NULL) {
        if (arena->page_size <= 0) {
            arena->page_size = MA_DEFAULT_PAGE_SIZE;
        }
        size_t alloc_size = (aligned_size > arena->page_size) ? (aligned_size*2) : arena->page_size;    // requested aligned_size can be > page_size, in that case allocate 2x of requested aligned_size
                                                                                        // TODO: update page_size if aligned_size > page_size ?
        page = init_page(alloc_size);
        if (prev_page != NULL) {
            prev_page->next = page; 
        }
        // On init, 'start' and 'end' will both point to same page, but this is fine, since allocation happens only through 'end' ptr
        if (arena->start == NULL) {
            arena->start = page;
        }
    }
    
    // Without this arena->end would never go forward (i.e. would always point to arena->start), if allocating after arena_reset()
    if (arena->end != page) { arena->end = page; }

    page->capacity += aligned_size;
    void* ret = page->start_free;
    page->start_free = (char*)page->start_free + aligned_size;
    return ret;
}

void arena_free(Arena* arena) 
{
    assert(arena != NULL);
    Page* current_page = arena->start;
    Page* next = NULL;
    while (current_page != NULL) {
        next = current_page->next;
        free(current_page);
        current_page = next;
    }
    arena->start = NULL;
    arena->end = NULL;
} 

void arena_reset(Arena* arena)
{
    assert(arena != NULL);
    Page* current_page = arena->start;
    while (current_page != NULL) {
        current_page->capacity = 0;
        current_page->start_free = current_page + sizeof(Page);     // reset free ptr to begining

        current_page = current_page->next;
    }
    arena->end = arena->start;
}

#endif // MARENA_IMPLEMENTATION
