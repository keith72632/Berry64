#include "uart.h"
#include "mbox.h"
#include "rand.h"
#include "delays.h"

void main()
{
    // set up serial console
    uart_init();
    uart_puts("if\n");
    wait_cycles(100000000000000);
    uart_puts("then\n");

    uart_puts("before dealy\n");
    wait_msec(1000000);
    uart_puts("after delay\n");
   
    
    // get the board's unique serial number with a mailbox call
    mbox[0] = 8*4;                  // length of the message
    mbox[1] = MBOX_REQUEST;         // this is a request message
    
    mbox[2] = MBOX_TAG_GETSERIAL;   // get serial number command
    mbox[3] = 8;                    // buffer size
    mbox[4] = 8;
    mbox[5] = 0;                    // clear output buffer
    mbox[6] = 0;

    mbox[7] = MBOX_TAG_LAST;

    // send the message to the GPU and receive answer
    if (mbox_call(MBOX_CH_PROP)) {
        uart_puts("My serial number is: ");
        uart_hex(mbox[6]);
        uart_hex(mbox[5]);
        uart_puts("\n");
    } else {
        uart_puts("Unable to query serial!\n");
    }
 

    // echo everything back
    while(1) {
        uart_send(uart_getc());
    }
}