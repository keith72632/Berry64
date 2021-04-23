#include "../includes/mem.h"
#include "../includes/atags.h"
#include "../includes/stdlib.h"
#include "../includes/uart.h"

extern unsigned char _end;

static int numPages;

//defines linked list struct type *page_list_t* with head and tail pointers
DEFINE_LIST(page);

//creates linked list struct type *page_list_t* defined above
IMPLEMENT_LIST(page);

static page_t * allPagesArray;

//defined with macros above
page_list_t freePages;



void mem_init(/*atag_t * atags*/)
{
    int pageArrayLen, kernelPages, i;
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
        allPagesArray[i].virt_addr_mapped = i * PAGE_SIZE;
        allPagesArray[i].flags.allocated = 1;
        allPagesArray[i].flags.kernel_page = 1;
    }
    //map the rest as unalloc and add to free list
    for(; i < numPages; i++)
    {
        allPagesArray[i].flags.allocated = 0;
        //function defined in macro IMPLEMENT_LIST. Appends each page 
        append_page_list(&freePages, &allPagesArray[i]);
    }

    uart_puts("Memory initalized to 2048 mb\n\n");

}