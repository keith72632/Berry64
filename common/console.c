#include "../includes/colors.h"
#include "../includes/uart.h"

void consoleError(char * string)
{
    uart_puts(KRED);
    uart_puts("[x]");
    uart_puts(string);
    uart_puts(KWHT);
}

void consolePrint(char * string)
{
    uart_puts(KGRN);
    uart_puts("[+]");
    uart_puts(string);
    uart_puts(KWHT);
}