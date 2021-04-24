#include "../includes/lfb.h"
#include "../includes/homeScreen.h"
#include "../includes/delays.h"
#include "../includes/uart.h"

/**********************************************************
*                   Private Functions                     *
***********************************************************/

/**********************************************************/

void homeScreenInit()
{
    clearScreen();
}

void cursor(int pos)
{
    drawRect(pos, 0, pos + (8 * ZOOM), (12 * ZOOM), 0x2e, 1);
    wait_msec(4900);
    drawRect(pos, 0, pos + (8 * ZOOM), (12 * ZOOM), 0x00, 1);
    wait_msec(4900);
}

void shell(int cursorPos)
{
    cursor(cursorPos);
}