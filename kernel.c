#include "uart.h"
#include "mbox.h"
#include "rand.h"
#include "delays.h"
#include "lfb.h"
#include "power.h"
#include "sd.h"

extern unsigned char _end;

extern int string_len;

void get_board_num();

void main()
{
    int string_start = (RES_WIDTH / 2);
    // set up serial console
    uart_init();
    lfb_init();

    // display an ASCII string on screen with PSF

    banner();

    // echo everything back

    //intitalize emmc and detect card type
    if(sd_init() == SD_OK)
    {
        // read the master boot record after bss segment
        if(sd_readblock(0, &_end, 1))
        {
            //dump to serial console
            uart_dump(&_end);
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

