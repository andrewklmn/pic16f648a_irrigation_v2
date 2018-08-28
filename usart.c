#include <xc.h>
#include <stdio.h>
#include <pic16f648a.h>
#include "usart.h"


void putch( char byte) {
    /* output one byte */
    while(!TXIF)    /* set when register is empty */
        continue;
    TXREG = byte;
}
/*
unsigned char getch() {
    char tmp;
    
    if (FERR || OERR) {
        CREN = 0;
        tmp = RCREG;
        tmp = RCREG;
        tmp = RCREG;
        CREN = 1;
    };
    
    return RCREG;
};
*/

/*
unsigned char getche(void) {
    unsigned char c;
    putch(c = getch());
    return c;
};
*/

void print_to_uart(char *text) {
	int ii;
	for(ii=0; text[ii]!='\0'; ii++) {
	   putch(text[ii]);
    };
    putch('\r');
    putch('\n');
}