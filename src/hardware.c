/** 
 *  @file hardware.c
 *  @brief Takes care of hardware initialisation
 *
 *  @author Thomas Evison
 *  @date 28/10/2020
 
                          ----------
                      Vdd |1      8| GND/VSS
      PIR sensor (in) GP5 |2      7| GP0 (in) DIP3
         SHUTTER (out)GP4 |3      6| GP1 (in) DIP1
                 (in) GP3 |4      5| GP2 (in) DIP2
                          ----------
 */

/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/
#include <xc.h>            /* XC8 General Include File */
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include "system.h"
#include "hardware.h"
#include <string.h>

/**
 *  Setup oscillator
 *  @author Thomas Evison
 *  @date 28/10/2020
 */
void Hardware_ConfigureOscillator(){
    /* TODO Add clock switching code if appropriate.  */

    /* Typical actions in this function are to tweak the oscillator tuning
    register, select new clock sources, and to wait until new clock sources
    are stable before resuming execution of the main project. */
    //OSCFRQ = 0B00000000;
}

/**
 *  Initialize hardware registers to setup IO, Interrupts, etc
 *  @author Thomas Evison
 *  @date 28/10/2020
 */
void Hardware_initIO(){
    
    /* setup I/O registers */
    TRISA = 0B00101011;            // set PORTA directions
    
    WPUA = TRISA;                  // weak pullups on all inputs
    ANSELA = 0x00;                 // Pin is assigned Digital I/O
    PORTA = 0x00;                  // set all outputs false
    
    PPSLOCKED = false;
    RA0PPS  = 0x00;
    PPSLOCKED = true;
    
    /* set interrupts */
    IOCAN = 0B00100000;            // look for falling edge on RA5
    IOCAP = 0x00;                  // ignore all rising edge
    INTCONbits.GIE = true;         // enable global interrupts
    INTCONbits.PEIE = true;        // enable peripheral interrupts
    INTCONbits.INTEDG = false;     // look for falling edge on Interrupt pin
    PIE0bits.IOCIE = true;         // Interrupt-on-Change Interrupt Enable bit
    PIE1 = 0x00;                   // turn
    PIE2 = 0x00;                   // off
    PIE3 = 0x00;                   // all other
    PIE4 = 0x00;                   // interrupts
}


/**
 *  Initialize hardware UART
 *  @author Thomas Evison
 *  @date 28/10/2020
 */
void Hardware_initUART(){
    
    /* choose Tx pin with PPS */
    PPSLOCKbits.PPSLOCKED = false;
    RA2PPS = 0b10100; // set RA2 as UART Tx
    PPSLOCKbits.PPSLOCKED = true;

    /* setup UART transmission */
    SYNC = 0; // configures the EUSART for asynchronous operation
    RC1STAbits.SPEN = 1; // configures the TX/CK I/O pin as an output
    TX1STAbits.TX9 = 0; // set 8 data bits
    TX1STAbits.BRGH = 0; // only use lower baudrate register
    TX1STAbits.SENDB = 0; // do not send break character
    SP1BRGL = 51; // set baud rate to 9615.4
    
    TX1STAbits.TXEN = 1; // enables the transmitter circuitry of the EUSART
    RC1STAbits.CREN = 1; // enables the receiver circuitry of the EUSART   
    PIE1bits.RCIE=true;  // enable UART receive interrupt 
                         // only enable Tx interrupt once data is ready to send
    

}

/**
 *  Send a single byte via UART
 *  @author Thomas Evison
 *  @date 28/10/2020
 */
void Hardware_UARTsendByte(uint8_t Tx) {
    while (!TXIF);
    TXREG = Tx;
}

/**
 *  Send string via hardware UART
 *  @author Thomas Evison
 *  @date 28/10/2020
 */
void Hardware_UARTsendString(char Tx[]) {
    for (int i = 0; i < strlen(Tx); i++) {
        while (!TXIF);
        TXREG = Tx[i];
    }
}
