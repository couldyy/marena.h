#ifndef MARENA_H
#define MARENA_H

// TODO: get rid of all includes, and include them only when some macro is defined
#include <assert.h>     // assert()
#include <stdint.h>     
#include <stdlib.h>     // malloc()
#include <string.h>     // memcpy()

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

#define MA_INT_ZEROED 0x1

#define MA_O_ARENA_DYNAMIC 0
#define MA_O_ARENA_STATIC 1
#define MA_O_DYNAMIC_PAGE_SIZE 2

#define MARENA_DYNAMIC_PAGE_SIZE_GROW_FACTOR 2

#define MARENA_PAGEMISS_FACTOR 0.005f
//#define PAGEMISS_PER_ALLOC(arena) ((float)(arena)->pagemiss_cnt_local / (float)(arena)->allocs_cnt_local)
#define MARENA_PAGEMISS_PER_ALLOC(arena) ((float)(arena)->_pagemiss_cnt / (float)(arena)->_allocs_cnt)   // global counters

// pointer to that struct is actual start of page, usable memory is at &page + sizeof(Page)
//  'free' contains start address of free memory in that page
typedef struct Page {
    void* start_free;   // pointer (offset from the begining) to the start of allocatable memory in page
    size_t capacity;    // bytes
    size_t size;        // bytes

    struct Page* next;
    //int is_zeroed;    // TODO: implement that field and check for it in arena_*_zero() functions, 
                        // to reduce memset() functions
} Page;

typedef struct {
    Page* start;    // TODO: some pages mey be full, in order not to iterate through them, maybe store some pointer to page that has free memory?
    Page* end;    
    size_t page_size;       // size for further allocations of page
    int flags;
    uint64_t _pagemiss_cnt;
    uint64_t _allocs_cnt;
#ifdef MARENA_DEBUG
    uint64_t _page_size_grows_cnt;
#endif
} Arena;



// initializes page
Page* init_page(size_t size);

// initializes arena on the heap
Arena* arena_init_heap(size_t size);

// allocates at least 'size' aligned bytes within 'arena' memory
void* arena__alloc_flags(Arena* arena, size_t size, int flags);

// allocates at least 'size' aligned bytes within 'arena' memory, initialized to 0
//void* arena_alloc_zero(Arena* arena, size_t size);

// Deallocates all pages in arena
// Note: does NOT deallocate 'arena' struct itself
void arena_free(Arena* arena);

// Marks all pages as free
void arena__reset_flags(Arena* arena, int flags);

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
#define arena_alloc(arena, size) arena__alloc_flags(arena, size, 0)

// TODO: performance is 10x worse then in arena_alloc(), optimise it
#define arena_alloc_zero(arena, size) arena__alloc_flags(arena, size, MA_INT_ZEROED)


// alloc with internal flags
void* arena__alloc_flags(Arena* arena, size_t size, int flags)
{
    assert(arena != NULL);
    size_t size_aligned = MA_ALIGN(size);

    Page* page;
    Page* prev_page;

    // init is a special case, no flags are checked, since in all of them at least 1 page must be allocated
    // also first page allocation doesnt count as page miss
    if (arena->start == NULL) {
        if (arena->page_size <= 0) { arena->page_size = MA_DEFAULT_PAGE_SIZE; }
        page = init_page(size_aligned > arena->page_size ? size_aligned : arena->page_size);    // TODO: if requested size > page_size, multiply by some factor?
        arena->start = page;
        arena->end = page;
        goto alloc;
    }

    page = arena->end;
    prev_page = NULL;
    while (page != NULL && page->capacity + size_aligned > page->size) {
        prev_page = page;
        page = page->next;
    }
    // just allocate new page, if there are no free space left in pages
    if (page == NULL) {
        if (arena->flags & MA_O_ARENA_STATIC) {
#ifdef MARENA_STATIC_RETURN_NULL_ON_FULL
            return NULL;
#else
            fprintf(stderr, "Not enough memory in the arena for requested size (Arena set to STATIC)\n");
            exit(1);
#endif //MARENA_STATIC_DONT_ABORT_ON_FULL

        }

        //arena->pagemiss_cnt_local += 1;
        arena->_pagemiss_cnt += 1;
        // grow page_size if allocations causes new page allocs frequently
        if ((arena->flags & MA_O_DYNAMIC_PAGE_SIZE) && (MARENA_PAGEMISS_PER_ALLOC(arena) >= MARENA_PAGEMISS_FACTOR)) {
            arena->page_size *= MARENA_DYNAMIC_PAGE_SIZE_GROW_FACTOR;  // TODO multiplication factor?
            // reset counter, since we care only about local data (after last page alloc)
            //arena->pagemiss_cnt_local = 0;
            //arena->allocs_cnt_local = 0;

        #ifdef MARENA_DEBUG
            arena->_page_size_grow_cnt++;
        #endif

        }
        size_t page_alloc_size = (size_aligned > arena->page_size) ? (size_aligned) : arena->page_size;    // TODO: if requested size > page_size, multiply by some factor?
                                                                                                           // TODO: update page_size if size > page_size ?
        page = init_page(page_alloc_size);
        if (prev_page != NULL) {
            prev_page->next = page; 
        }
        //arena->end = page;
    }

alloc:
    //arena->allocs_cnt_local++;

    // after reset there may be some pages already allocated and linked, update in that case
    if (arena->end != page) { arena->end = page; }
    arena->_allocs_cnt++;
    page->capacity += size_aligned;
    void* ret = page->start_free;
    if (flags & MA_INT_ZEROED) {
        memset(ret, 0, size_aligned);
    }
    page->start_free = (char*)page->start_free + size_aligned;
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

// reset WITH flag
void arena__reset_flag(Arena* arena, int flags)
{
    assert(arena != NULL);
    for (Page* page = arena->start; page != NULL; page = page->next) {
        if (flags & MA_INT_ZEROED) {
            //memset((char*)page + sizeof(Page), 0, page->capacity);  // memset only capacity bytes
            memset((char*)page + sizeof(Page), 0, page->size);  // TODO: test this, does performance and quality differ?
        }

        page->capacity = 0;
        page->start_free = page + sizeof(Page);     // reset free ptr to begining
    }
    arena->end = arena->start;
}

#define arena_reset(arena) arena__reset_flag(arena, 0)

#define arena_reset_zero(arena) arena__reset_flag(arena, MA_INT_ZEROED)

#endif // MARENA_IMPLEMENTATION
