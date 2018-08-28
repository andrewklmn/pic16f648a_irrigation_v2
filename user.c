/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>         /* XC8 General Include File */
#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "user.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

void InitApp(void)
{
    OPTION_REGbits.PSA = 1;         // watchdog to 2.3s
    OPTION_REGbits.PS = 0b111;
    
    /* Setup analog functionality and port direction */
    //CMCON = 0b111; //Disable PORTA Comparators
    TRISA = 0b00000111;         // RA0-1-2 like input   
    TRISB = 0b10000010;         // RB1 like RX input setup
    
    /* Initialize peripherals */
    VRCON = 0b11001111;         // Vref setup for day mode
    CMCON = 0b00000010;         // CMP setup
    PORTB = 0;                  // All ventils OFF

    /* Enable interrupts */
    INTCON = 0b11000000; 
    RCIE=1;              // UART receive interrupt enable    
    
}

