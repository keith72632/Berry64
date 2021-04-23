#include "../includes/uart.h"
#include "../includes/mbox.h"
#include "../includes/rand.h"
#include "../includes/delays.h"
#include "../includes/lfb.h"
#include "../includes/power.h"
#include "../includes/sd.h"
#include "../includes/atags.h"
#include "../includes/mem.h"
#include "../includes/console.h"

extern unsigned char _end;

extern int string_len;

void get_board_num();


void main(int r0, int r1, int atags)
{
    // set up serial console
    uart_init();

    //set up framebuffer
    lfb_init();

    // animated banner
    banner();

    mem_init(/*(atag_t *)atags*/);

    //intitalize emmc and detect card type
    if(sd_init() == SD_OK)
    {
        // read the master boot record after bss segment
        if(sd_readblock(0, &_end, 1))
        {
            //dump to serial console
            //uart_dump(&_end);
            consolePrint("SD read successful\n");
        }

    } else if(sd_init() == SD_TIMEOUT) {
        uart_puts("Sd timeout\n");
    } else if(sd_init() == SD_ERROR) {
        uart_puts("Sd error\n");
    }
    while(1) {
        uart_send(uart_getc());
    }
}

