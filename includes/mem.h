#include "atags.h"
#include "lists.h"


#define PAGE_SIZE 4096
#define KERNEL_HEAP_SIZE (1024*1024)

/*********************************************************************
 *                         Structures                                *
 *********************************************************************/

/* Paging Structures */
typedef struct {
    char allocated         :1;       //allocated for something else
    char kernel_page       :1;       //page is part of kernel
    char kernel_heap_page  :1;
    int reserved           :29;
} page_flags_t;

typedef struct page {
    int virt_addr_mapped;
    page_flags_t flags;
    DEFINE_LINK(page);           //creates linked list with next and prev pointers
} page_t;

/* Heap Structures */
typedef struct heap_segment {
    struct heap_segment * next;
    struct heap_segment * prev;
    int isAllocated;
    int segmentSize;
} heap_segment_t;

void mem_init(/*atag_t * atags*/);

void * alloc_page(void);
void free_page(void * ptr);

void * kmalloc(int bytes);
void kfree(void *ptr);