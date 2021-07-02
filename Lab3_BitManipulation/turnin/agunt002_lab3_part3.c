/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	
	unsigned char tempA = 0x00;
	unsigned char snsr = 0x00;
    /* Insert your solution below */
    while (1) {
	snsr  = 0; //Reset the sum everytime is loops;
	tempA = PINA;

	//If loops to check and display
	
	if(((tempA & 0x30) == 0x30) && (tempA & 0x40) != 0x40){
		snsr = snsr | 0x80;
  	}
	if(((tempA & 0x0D) == 0x0D) || ((tempA & 0x0E) == 0x0E) || ((tempA & 0x0F) == 0x0F))
		snsr = 0x3F;
	else if(((tempA & 0x0A) == 0x0A) || ((tempA & 0x0B) == 0x0B) || ((tempA & 0x0C) == 0x0C))
		snsr = 0x3E;
	else if (((tempA & 0x07) == 0x07) || ((tempA & 0x08) == 0x08) || ((tempA & 0x09) == 0x09))
		snsr = 0x3C;
	else if(((tempA & 0x05) == 0x05) ||  ((tempA & 0x06) == 0x06))
		snsr = 0x38;
	else if(((tempA & 0x03) == 0x03) || ((tempA & 0x04) == 0x04))
		snsr = 0x70;
	else if(((tempA & 0x01) == 0x01) || ((tempA &  0x02) == 0x02))
		snsr = 0x60;
	else
		snsr = 0x40;
	
	PORTC = snsr;
    }
    return 1;
}
