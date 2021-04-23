#include "../includes/mem.h"
#include "../includes/atags.h"
#include "../includes/stdlib.h"
#include "../includes/uart.h"
#include "../includes/console.h"

extern unsigned char _end;


//defines linked list struct type *page_list_t* with head and tail pointers
DEFINE_LIST(page);

//creates linked list struct type *page_list_t* defined above with next and prev pointers
IMPLEMENT_LIST(page);


/****Static Varaibles****/
static page_t * allPagesArray;
static int numPages;
static heap_segment_t * heapSegmentListHead;

/****Static Functions****/
static void heap_init(int heap_start);

//defined with macros above
page_list_t freePages;


/*************************************************************
 *                    Paging Functions                       *
 *************************************************************/

void mem_init(/*atag_t * atags*/)
{
    int pageArrayLen, pageArrayEnd, kernelPages, i;
    long long int memSize;

    //get total number of pages
    memSize = getMemSize();
    numPages = memSize / PAGE_SIZE;

    //alloc space for all thoe page's metadata. Start block just aftern kernel page is finished
    pageArrayLen = sizeof(page_t) * numPages;

    //Location of start of array of pages
    allPagesArray = (page_t *)&_end;
    bzero(allPagesArray, pageArrayLen);
    INITIALIZE_LIST(freePages);

    //iterate over all pages and mark them with flags
    //start with kernel pages
    kernelPages = ((int)&_end) / PAGE_SIZE;
    for(i = 0; i < kernelPages; i++)
    {
        allPagesArray[i].virt_addr_mapped = i * PAGE_SIZE;  //identity map kernel pages
        allPagesArray[i].flags.allocated = 1;
        allPagesArray[i].flags.kernel_page = 1;
    }
    // Reserve 1 MB for the kernel heap
    for(; i < kernelPages + (KERNEL_HEAP_SIZE / PAGE_SIZE); i++){
        allPagesArray[i].virt_addr_mapped = i * PAGE_SIZE;    // Identity map the kernel pages
        allPagesArray[i].flags.allocated = 1;
        allPagesArray[i].flags.kernel_heap_page = 1;
    }
    //map the rest as unalloc and add to free list
    for(; i < numPages; i++)
    {
        allPagesArray[i].flags.allocated = 0;
        //function defined in macro IMPLEMENT_LIST. Appends each page 
        append_page_list(&freePages, &allPagesArray[i]);
    }

    // Initialize the heap
    pageArrayEnd = (int)&_end + pageArrayLen;
    heap_init(pageArrayEnd);    
    consolePrint("Heap intitalized\n\n");
    consolePrint("Memory initalized to 1048 mb\n\n");

}

void * allocPage(void)
{
    page_t * page;
    void * pageMem;

    //return freePages->size. If zero, return nothing
    if(size_page_list(&freePages) == 0) return 0;

    //get a free page
    page = pop_page_list(&freePages);
    page->flags.kernel_page = 1;
    page->flags.allocated = 1;

    //get the address the physical page metadata refers to
    pageMem = (void *)((page-allPagesArray) * PAGE_SIZE);

    //zero out the page, big secruity flaw not to do this
    bzero(pageMem, PAGE_SIZE);

    return pageMem; 
}

void freePage(void * ptr)
{
    page_t * page;

    //get page metadata from the physical address. Indexing. Same as allPagesArray[((int)ptr / PAGE_SIZE)]
    page = allPagesArray + ((int)ptr / PAGE_SIZE);

    //mark the page as free
    page->flags.allocated = 0;
    append_page_list(&freePages, page);
}


/*************************************************************
 *                     Heap Functions                        *
 *************************************************************/

void * kmalloc(int bytes)
{
    heap_segment_t * current, *best = NULL;
    int diff, bestDiff = 0x7fffffff;      //maxed int sign

    //Add the header to the number of bytes we need and make the size 16 byte aligned
    bytes += sizeof(heap_segment_t);
    bytes += bytes % 16 ? 16 - (bytes % 16) : 0;

    //find the allocation that is closest in size to this request
    for(current = heapSegmentListHead; current != NULL; current = current->next)
    {
        diff = current->segmentSize - bytes;
        if(!current->isAllocated && diff < bestDiff && diff >= 0)
        {
            best = current;
            bestDiff = diff;
        }
    }

    //IF there is no free memory 
    if(best == NULL) return NULL;

    // If the best difference we could come up with was large, split up this segment into two.
    // Since our segment headers are rather large, the criterion for splitting the segment is that
    // when split, the segment not being requested should be twice a header size
    if(bestDiff > (int)(2 * sizeof(heap_segment_t)))
    {
        bzero(((void*)(best)) + bytes, sizeof(heap_segment_t));
        current = best->next;
        best->next = ((void*)(best)) + bytes;
        best->next->next = current;
        best->next->prev = best;
        best->next->segmentSize = best->segmentSize - bytes;
        best->segmentSize = bytes;
    }

    best->isAllocated = 1;

    return best + 1;
}

void kfree(void*ptr)
{
    heap_segment_t * seg = ptr - sizeof(heap_segment_t);
    seg->isAllocated = 0;

    //try to coalesce segments to the left
    while(seg->prev != NULL && !seg->prev->isAllocated)
    {
        seg->prev->next = seg->next;
        seg->prev->segmentSize += seg->segmentSize;
        seg = seg->prev;
    }

    //try to coalesce segments to the right
    while(seg->next != NULL && !seg->next->isAllocated)
    {
        seg->segmentSize += seg->next->segmentSize;
        seg->next = seg->next->next;
    }
}

static void heap_init(int heap_start) {
   heapSegmentListHead = (heap_segment_t *) heap_start;
   bzero(heapSegmentListHead, sizeof(heap_segment_t));
   heapSegmentListHead->segmentSize = KERNEL_HEAP_SIZE;
}