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
#include "../includes/homeScreen.h"

#define ENTER 0x1C

extern unsigned char _end;

extern int string_len;

void get_board_num();


void main(int r0, int r1, int atags)
{
    // set up serial console
    uart_init();

    //set up framebuffer
    lfb_init();

    mem_init(/*(atag_t *)atags*/);

    //intitalize emmc and detect card type
    initEMMC();

    // animated banner
    banner();

    //press any key
    while(!uart_getc());

    //home screen
    homeScreenInit();

    while(1) {
        uart_send(uart_getc());        
    }
}


