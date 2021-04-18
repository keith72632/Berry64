#include "gpio.h"
#include "uart.h"

/*Mailbox message buffer*/
volatile unsigned int __attribute__((aligned(16))) mbox[36];

#define VIDEOCORE_MBOX  (MMIO_BASE+0x0000B880)
#define MBOX_READ       ((volatile unsigned int*)(VIDEOCORE_MBOX+0x0))
#define MBOX_POLL       ((volatile unsigned int*)(VIDEOCORE_MBOX+0x10))
#define MBOX_SENDER     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x14))
#define MBOX_STATUS     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x18))
#define MBOX_CONFIG     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x1C))
#define MBOX_WRITE      ((volatile unsigned int*)(VIDEOCORE_MBOX+0x20))
#define MBOX_RESPONSE   0x80000000
#define MBOX_FULL       0x80000000
#define MBOX_EMPTY      0x40000000


/* Make another mailbox call. Returns 0 on failure, non-zero on success */
int mbox_call(unsigned char ch)
{
    unsigned int r = (((unsigned int)((unsigned long)&mbox) & ~0xf) | (ch & 0xf));
    /* Wait until mailbox is ready to be written to */
    do {
        asm volatile("nop");
    } while(*MBOX_STATUS & MBOX_FULL);
    /* write the address of our message to the mailbox with channel identifier */
    *MBOX_WRITE = r;
    uart_puts("Mailbox request sent\n");
    while(1)
    {
        /* Is there a response? */
        do {
            asm volatile("nop");
        } while(*MBOX_STATUS & MBOX_EMPTY);
        /* Is there a response? */
        if(r == *MBOX_READ){
            /* Is it a valid successful respnse? */
            return mbox[1] == MBOX_RESPONSE;
            uart_puts("Mailbox response recieved\n");
        }
    }
    return 0;
}