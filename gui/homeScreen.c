#include "../includes/lfb.h"
#include "../includes/homeScreen.h"

/**********************************************************
*                   Private Functions                     *
***********************************************************/
static void shellInit();
/**********************************************************/

void homeScreenInit()
{
    clearScreen();
    shellInit();
}

static void shellInit()
{
    drawChar('X', (1280 /2), (720 / 2), 0x0f, 1);
}