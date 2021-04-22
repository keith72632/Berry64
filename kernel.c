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
    //lfb_print(20, 20, "Screen Resolution: ");
    drawCircle(640, 360, 50, RED, 1);
    drawCircle(640, 360, 40, BLACK, 1);
    drawCircle(640, 360, 30, RED, 1);
    drawCircle(640, 360, 20, BLACK, 1);
 
    drawRect(400,50,825,125,0x02,0);
//    drawRect(300,300,350,350,0x2e,1);
    drawString(450, 75, "Dingleberry OS", 0x02, 3);
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

