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
	unsigned char cntavail = 0x00; // Temporary variable to hold the value of C
	while(1) {
		// 1) Read input
		tmpA = PINA & 0x0F;
		// 2) Perform computation
		// if PA0 is 0, set PC0 = 1, else = 0
		if (tmpA == 0x01 || tmpA == 0x02 || tmpA == 0x04 || tmpA == 0x08 ) { // True if 1 space is occupied
			cntavail = 0x03; // Sets cntavail to +1
		}
		else if (tmpA == 0x03 || tmpA == 0x06 || tmpA == 0x0C || tmpA == 0x05 || tmpA == 0x0A || tmpA == 0x09) {
			cntavail = 0x02;
		}
		else if (tmpA == 0x07 || tmpA == 0x0E || tmpA == 0x0D || tmpA == 0x0B) {
			cntavail = 0x01;
		}
		else if (tmpA == 0x0F) {
			cntavail = 0x80;
		}
		else {
			cntavail = 0x04;
		}
	// 3) Write output
	PORTC = cntavail;
	cntavail = 0x00;
	}
	return 0;
}

