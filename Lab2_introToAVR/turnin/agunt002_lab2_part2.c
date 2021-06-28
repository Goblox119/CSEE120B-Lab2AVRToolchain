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
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs, initialize to 0s
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
	unsigned char tmpA1 = 0x00;
	unsigned char tmpA2 = 0x00;
	unsigned char tmpA3 = 0x00;
	unsigned char cntavail = 0x00; // Temporary variable to hold the value of C
	while(1) {
		// 1) Read input
		tmpA = PINA & 0x01;
		tmpA1 = PINA & 0x02;
		tmpA2 = PINA & 0x04;
		tmpA3 = PINA & 0x08;
		// 2) Perform computation
		// if PA0 is 0, set PC0 = 1, else = 0
		if (tmpA == 0x00) { // True if PA0 is 0
			cntavail = cntavail + 1; // Sets cntavail to +1
		}
		if (tmpA1 == 0x00) {
			cntavail = cntavail + 1;
		}
		if (tmpA2 == 0x00) {
			cntavail = cntavail + 1;
		}
		if (tmpA3 == 0x00) {
			cntavail = cntavail + 1;
		}	
	// 3) Write output
	PORTC = cntavail;
	cntavail = 0x00;
	}
	return 0;
}

