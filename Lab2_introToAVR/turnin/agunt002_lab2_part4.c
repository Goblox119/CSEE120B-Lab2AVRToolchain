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
#include <stdlib.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif	

int main(void) {
		
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as inputs
	DDRC = 0x00; PORTC = 0xFF; // Configure port C's 8 pins as inputs
	DDRD = 0xFF; PORTD = 0x00; // Configure port D's 8 pins as outputs, initialize to 0s


	unsigned char tmpA = 0x00, tmpB = 0x00, tmpC = 0x00, tmpD = 0x00; // Temporary variables to hold the values of A, B, C and D
	unsigned char totalW = 0x00;
	while(1) {
		// 1) Read input
		tmpA = PINA;
		tmpB = PINB;
		tmpC = PINC;
		tmpD = 0x00; //reset value pf output
		totalW = tmpA + tmpB + tmpC;
        tmpD = totalW / 15;
        tmpD = tmpD << 2; //shift lft two bits
		// 2) Permoft the computation 
		if(totalW > 140) {
			tmpD = tmpD + 0x01;
		}
        if(((tmpA - tmpC) > 0x50) || ((tmpC - tmpA) > 0x50)){
            tmpD = tmpD + 0x02;
		}
		// 3) Write to the outputs
        PORTD = tmpD;
        tmpD = 0x00; //reset
	}
	return 0;
}
