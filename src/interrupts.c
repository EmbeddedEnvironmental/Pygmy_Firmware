/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/
#include <xc.h>         /* XC8 General Include File */
#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */
#include "pygmy.h"

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/
void __interrupt() isr(void)
{
    /* Determine which flag generated the interrupt */
    if(IOCIF && IOCAF5){
        GIE = false;          // disable global interrupts
        IOCAF5 = false;       // Clear Interrupt Flag
        Pygmy_TriggeredPIR(); // Fire Camera
        GIE = true;           // enable global interrupts
    }
    else{
        // Unhandled interrupts
    }

}



