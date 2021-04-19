#include "uart.h"
#include "mbox.h"
#include "rand.h"

void main()
{
    // set up serial console
    uart_init();
    rand_init();
    int x = rand(10, 100);
    int y = 10;
    int size = sizeof(y);
    
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
    uart_puts("Size of int y: ");
    uart_hex(y);

    // echo everything back
    while(1) {
        uart_send(uart_getc());
    }
}