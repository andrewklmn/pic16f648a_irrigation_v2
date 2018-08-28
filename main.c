/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>        /* For uint8_t definition */
//#include <stdbool.h>       /* For true/false definition */
#include <stdio.h>

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */
#include "usart.h"
#include "state.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/


int delay_counter = 0;              // counter for checking day and night state 



int calculate_ventil_time(unsigned char eeprom_data){
    return (DEFAULT_DURATION + eeprom_data -125)*TIME_UNIT;
};


/* i.e. uint8_t <variable_name>; */

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
void main(void)
{
    /* Configure the oscillator for the device */
    //ConfigureOscillator();
    

    /* Initialize I/O and Peripherals for application */
    InitApp();

    RESET_WIFI_STATE = 0; 
    RESET_WIFI_PIN = 0;     // Reset wifi module started

    init_comms();
    
    
    state.night_is_on = !C1OUT;              // check is night or day on start
    
    state.v1_time = 0;
    state.v2_time = 0;
    state.v3_time = 0;
    state.v4_time = 0;
    
    
    state.irrigation_disabled = eeprom_read(0x04);  // read last setting
    
    print_to_uart("Welcome to irrigation system v.2.0");
    print_to_uart("http://github.com/andrewklmn/pic16f648a_irrigation");
    
    INFO_LED = 1;
    __delay_ms(100);
    INFO_LED = 0;
    __delay_ms(200);
    INFO_LED = 1;
    __delay_ms(100);
    INFO_LED = 0;
    __delay_ms(200);
    INFO_LED = 1;
    __delay_ms(100);
    INFO_LED = 0;
    __delay_ms(700);
    
    RESET_WIFI_PIN = 1;     // Reset wifi module ended 
    
    CLRWDT();
    
    while(1)
    {
        /* check day or night changes with delay*/
        //if (RA0==1) {
        if( !C1OUT ) { 
            // night signal from sensor
            if (state.night_is_on==0) {
                // check if it's real state change or just noise
                delay_counter++;
                INFO_LED = 1;
                __delay_ms(500);
                INFO_LED = 0;
                __delay_ms(500);
                CLRWDT();
                if ( delay_counter > DAY_NIGHT_SWITCH_DELAY) {
                    state.night_is_on = 1;
                    delay_counter = 0;
                    // set working time for ventils in minutes
                    state.v1_time = calculate_ventil_time(eeprom_read(0x00));
                    state.v2_time = calculate_ventil_time(eeprom_read(0x01));
                    state.v3_time = calculate_ventil_time(eeprom_read(0x02));
                    state.v4_time = calculate_ventil_time(eeprom_read(0x03));
                    VRCON = 0b11000011;         // set Vref lower for hysteresis
                };
            } else {
                delay_counter = 0;
                INFO_LED = 1;
                __delay_ms(30);
                INFO_LED = 0;
                __delay_ms(50);
                INFO_LED = 1;
                __delay_ms(30);
                INFO_LED = 0;
                __delay_ms(890);
            };
        } else {
            // day signal from sensor
            if (state.night_is_on==1) {
                // check if it's real state change or just noise
                delay_counter++;
                INFO_LED = 1;
                __delay_ms(500);
                INFO_LED = 0;
                __delay_ms(500);
                CLRWDT();
                if (delay_counter > DAY_NIGHT_SWITCH_DELAY) {
                    state.night_is_on = 0;
                    delay_counter = 0;
                    // set working time for ventils in minutes
                    state.v1_time = calculate_ventil_time(eeprom_read(0x00));
                    state.v2_time = calculate_ventil_time(eeprom_read(0x01));
                    state.v3_time = calculate_ventil_time(eeprom_read(0x02));
                    state.v4_time = calculate_ventil_time(eeprom_read(0x03));
                    VRCON = 0b11001111;       // set Vref higher for hysteresis
                };
            } else {
                delay_counter = 0;
                INFO_LED = 1;
                __delay_ms(30);
                INFO_LED = 0;
                __delay_ms(970);
            };
        };
        
        
        // reset time if irrigation disabled
        if ( state.irrigation_disabled == 1) {
            state.v1_time = 0;
            state.v2_time = 0;
            state.v3_time = 0;
            state.v4_time = 0;
        };
        
        /* work cycle */
        if (state.v1_time > 0 ) {
            VENTIL_1 = 1;
            state.v1_time--;
            VENTIL_2 = 0;
            VENTIL_3 = 0;
            VENTIL_4 = 0;
        } else if ( state.v2_time > 0 ) {
            VENTIL_2 = 1;
            state.v2_time--;
            VENTIL_3 = 0;
            VENTIL_4 = 0;
            VENTIL_1 = 0;
        } else if ( state.v3_time > 0  ) {
            VENTIL_3 = 1;
            state.v3_time--;
            VENTIL_1 = 0;
            VENTIL_4 = 0;
            VENTIL_2 = 0;
        } else if ( state.v4_time > 0  ) {
            VENTIL_4 = 1;
            state.v4_time--;
            VENTIL_1 = 0;
            VENTIL_2 = 0;
            VENTIL_3 = 0;
        } else {
            PORTB = 0;
        };
        
        CLRWDT();
        RESET_WIFI_PIN = 1; 
    };

};

