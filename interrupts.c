/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#include <xc.h>         /* XC8 General Include File */


#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */
#include <stdio.h>
#include "usart.h"
#include "state.h"
#include "user.h"

extern int calculate_ventil_time(unsigned char);

unsigned char offset;
int work_duration;

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

/* Baseline devices don't have interrupts. Note that some PIC16's 
 * are baseline devices.  Unfortunately the baseline detection macro is 
 * _PIC12 */
#ifndef _PIC12


void __interrupt() isr(void)
{

    if ((RCIE)&&(RCIF)) { // ???? ???-?? ?????? ? ???????? UART ? ????????? ?????
        if(!RX9D && !OERR && !FERR) {   // ???? ??? ??????
            c = RCREG;
            if ( c  == 0x0d || c  == 0x0a ) { //?????? ?????? ????? ?????? ??? ??????? ???????
                a[i] = '\0';
                // ???????? ???????????? ? ????????? ????????, ??????? ??????
                if (a[0]=='A' && a[1]=='T') { //???? ??? ????????, ?? ????????????   
                    if (a[2]=='\0') { // ???????? ???????? ??????? ?????????? ?????? ??
                        print_to_uart("Ok");
                    };
                    if (a[2]==' ' && a[3]=='H' && a[4]=='\0') { // ???????? ???????? ??????? ?????????? ?????? ??
                        print_to_uart("AT   -> Ok");
                        print_to_uart("AT H - this help");
                        print_to_uart("AT E - enable irrigation");
                        print_to_uart("AT D - disable irrigation");
                        print_to_uart("AT W - start irrigation cycle");
                        print_to_uart("AT R - stop irrigation cycle");
                        print_to_uart("AT S - state: night_on|irr_disabled|v1_time|v2_time|v3_time|v4_time");
                        print_to_uart("AT +N - increase working time N-ventil");
                        print_to_uart("AT -N - degree working time N-ventil");
                        print_to_uart("AT DN - disable N-ventil");
                        print_to_uart("AT EN - enable N-ventil");
                        print_to_uart("AT ON - open N-ventil for default working time");
                        print_to_uart("AT CN - close N-ventil");
                        print_to_uart("AT T - show working time of ventils");
                        
                    };
                    if (a[2]==' ' && a[3]=='R' && a[4]=='\0') { // work cycle stoped
                        state.v1_time = 0;
                        state.v2_time = 0;
                        state.v3_time = 0;
                        state.v4_time = 0;
                        print_to_uart("Watering cycle stopped");
                    };
                    if (a[2]==' ' && a[3]=='W' && a[4]=='\0') { // work cycle started
                        state.v1_time = calculate_ventil_time(eeprom_read(0x00));
                        state.v2_time = calculate_ventil_time(eeprom_read(0x01));
                        state.v3_time = calculate_ventil_time(eeprom_read(0x02));
                        state.v4_time = calculate_ventil_time(eeprom_read(0x03));
                        print_to_uart("Watering cycle started");
                    };
                    /*
                    if (a[2]==' ' && a[3]=='N' && a[4]=='\0') { // day or night
                        if( state.night_is_on == 1 ) {
                            print_to_uart("night");
                        } else {
                            print_to_uart("day");
                        };
                    };
                     */
                    if (a[2]==' ' && a[3]=='D' && a[4]=='\0') { // disable irrigation
                         state.irrigation_disabled = 1;
                         eeprom_write(0x04,1);
                        print_to_uart("Watering disabled");
                    };
                    if (a[2]==' ' && a[3]=='E' && a[4]=='\0') { // ????????????? ?????
                        state.irrigation_disabled = 0;
                        eeprom_write(0x04,0);
                        print_to_uart("Watering enabled");
                    };
                    if (a[2]==' ' && a[3]=='T' && a[4]=='\0') { // show working time of ventils
                        printf( "%d|%d|%d|%d\r\n",
                            calculate_ventil_time(eeprom_read(0x00)),
                            calculate_ventil_time(eeprom_read(0x01)),
                            calculate_ventil_time(eeprom_read(0x02)),
                            calculate_ventil_time(eeprom_read(0x03))
                        );
                    };
                    if (a[2]==' ' && a[3]=='S' && a[4]=='\0') { // current state report
                        printf("%d|%d|%d|%d|%d|%d\r\n",
                                state.night_is_on,
                                state.irrigation_disabled,
                                state.v1_time,
                                state.v2_time,
                                state.v3_time,
                                state.v4_time
                        );
                    };
                    if (a[2]==' ' && a[3]=='+') { //AT +  
                        offset = 4;
                        switch(a[4]){
                            case '1':
                                offset = 0;
                                break;
                            case '2':
                                offset = 1;
                                break;
                            case '3':
                                offset = 2;
                                break;
                            case '4':
                                offset = 3;
                                break;
                        };
                        if (offset < 4 ) {
                            if ((calculate_ventil_time(eeprom_read(offset)) + 1) <= MAX_VENTIL_OPEN_TIME ) {
                                eeprom_write(offset,eeprom_read(offset) + 1);
                            };
                            printf( "%d|%d|%d|%d\r\n",
                                calculate_ventil_time(eeprom_read(0x00)),
                                calculate_ventil_time(eeprom_read(0x01)),
                                calculate_ventil_time(eeprom_read(0x02)),
                                calculate_ventil_time(eeprom_read(0x03))
                            );
                        };
                    };
                    if (a[2]==' ' && a[3]=='-') { //AT -  ??????????? ?????
                        offset = 4;
                        switch(a[4]){
                            case '1':
                                offset = 0;
                                break;
                            case '2':
                                offset = 1;
                             break;
                            case '3':
                                offset = 2;
                                break;
                            case '4':
                                offset = 3;
                                break;
                        };
                        if (offset < 4 ) {
                            if ((calculate_ventil_time(eeprom_read(offset)) -1) >= 0 ) {
                                eeprom_write( offset, eeprom_read(offset) - 1 );
                            };
                            printf( "%d|%d|%d|%d\r\n",
                                calculate_ventil_time(eeprom_read(0x00)),
                                calculate_ventil_time(eeprom_read(0x01)),
                                calculate_ventil_time(eeprom_read(0x02)),
                                calculate_ventil_time(eeprom_read(0x03))
                            );
                        };
                    };
                    if (a[2]==' ' && a[3]=='D' && a[4]!='\0') { //AT +  ??????????? ????
                        offset = 4;
                        switch(a[4]){
                            case '1':
                                offset = 0;
                                state.v1_time = 0;
                                break;
                            case '2':
                                offset = 1;
                                state.v2_time = 0;
                                break;
                            case '3':
                                offset = 2;
                                state.v3_time = 0;
                                break;
                            case '4':
                                offset = 3;
                                state.v4_time = 0;
                                break;
                        };
                        
                        if (offset < 4 ) {
                            eeprom_write( offset, 125 - DEFAULT_DURATION );
                            printf( "%d|%d|%d|%d\r\n",
                                calculate_ventil_time(eeprom_read(0x00)),
                                calculate_ventil_time(eeprom_read(0x01)),
                                calculate_ventil_time(eeprom_read(0x02)),
                                calculate_ventil_time(eeprom_read(0x03))
                            );
                        };
                    };
                    if (a[2]==' ' && a[3]=='E' && a[4]!='\0') { //AT +  ??????????? ????
                        offset = 4;
                        switch(a[4]){
                            case '1':
                                offset = 0;
                                break;
                            case '2':
                                offset = 1;
                                break;
                            case '3':
                                offset = 2;
                                break;
                            case '4':
                                offset = 3;
                                break;
                        };
                        
                        if (offset < 4 ) {
                            if (eeprom_read(offset) == (125 - DEFAULT_DURATION)) {
                                eeprom_write( offset, 125 );
                            };
                            printf( "%d|%d|%d|%d\r\n",
                                calculate_ventil_time(eeprom_read(0x00)),
                                calculate_ventil_time(eeprom_read(0x01)),
                                calculate_ventil_time(eeprom_read(0x02)),
                                calculate_ventil_time(eeprom_read(0x03))
                            );
                        };
                    };
                    if (a[2]==' ' && a[3]=='O' && a[4]!='\0') { //AT +  ??????????? ????
                        printf("Tap ");
                        switch(a[4]){
                            case '1':
                                state.v1_time = calculate_ventil_time(eeprom_read(0x00));
                                printf("1");
                                break;
                            case '2':
                                state.v2_time = calculate_ventil_time(eeprom_read(0x01));
                                printf("2");
                                break;
                            case '3':
                                state.v3_time = calculate_ventil_time(eeprom_read(0x02));
                                printf("3");
                                break;
                            case '4':
                                state.v4_time = calculate_ventil_time(eeprom_read(0x03));
                                printf("4");
                                break;
                            default:
                                printf("was not");
                                break;
                        };
                        printf(" opened.\r\n");
                    };
                    if (a[2]==' ' && a[3]=='C' && a[4]!='\0') { //AT +  ??????????? ????
                        printf("Tap ");
                        switch(a[4]){
                            case '1':
                                state.v1_time = 0;
                                printf("1");
                                break;
                            case '2':
                                state.v2_time = 0;
                                printf("2");
                                break;
                            case '3':
                                state.v3_time = 0;
                                printf("3");
                                break;
                            case '4':
                                state.v4_time = 0;
                                printf("4");
                                break;
                            default:
                                printf("was not");
                                break;
                        };
                        printf(" closed.\r\n");
                    };
                }; 
                a[0] = '\0'; 
                i = 0;
            } else {
                if (i==16){ 
                    a[0] = c;
                    i = 0;
                } else { 
                    a[i] = c;
                    i++;
                };
            };
            
        } else {
            
            c = RCREG;
            c = RCREG;
            CREN = 0;
            NOP();
            NOP();
            CREN = 1;
        };
    };

}

#endif


