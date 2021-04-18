#include "uart.h"
#include "mbox.h"

void main()
{

    //set up serial console
    uart_init();

    //get boards unique serial number witha mailbox call
    mbox[0] = 8 * 4;                       //Length of message
    mbox[1] = MBOX_REQUEST;                //this is a request
    mbox[2] = MBOX_TAG_GETSERIAL;          //get the serial number command
    mbox[3] = 8;                           //buffer size
    mbox[4] = 8;                           //
    mbox[5] = 0;                           // Clear output
    mbox[6] = 0;                

    mbox[7] = MBOX_TAG_LAST;

    // send message to the gpu and recieve answer
    if(mbox_call(MBOX_CH_PROP))
    {
        uart_puts("My serial number is: ");
        uart_hex(mbox[6]);
        uart_hex(mbox[5]);
        uart_puts("\n");
    } else {
        uart_puts("Unable to query serial!\n");
    }


    while(1){ uart_send(uart_getc()); };
}
