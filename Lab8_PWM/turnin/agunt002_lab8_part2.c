/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	DEMO LINK: https://drive.google.com/file/d/1Hunku8Rr_dU8X4CETC2Of9Nc3ymuyYM1/view?usp=sharing
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

// 0.954 hz is lowest frequency possible with this function 
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency){
	static double current_frequency; //Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uniterrupted
	if(frequency != current_frequency)
	{
		if(!frequency) // stops timer/counter 
		{ 
			TCCR3B &= 0x08;
		} 
		else // resumes/continues timer/counter 
		{ 
			TCCR3B |= 0x03; 
		} 

		//prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { 
			OCR3A = 0xFFFF; 
		}

		// prevents OCR3A from underflowings, using prescaler 64
		// 31250 is largest frequency that will not result in underflow
		else if(frequency > 31250) { 
			OCR3A = 0x0000; 
		}
		//set OCR3A based on desired frequency
		else { 
			OCR3A = (short)(8000000 / (128 * frequency)) - 1; 
		}
		TCNT3 = 0; // resets counter
		current_frequency = frequency; // Updates the curr frequency
	}
}

void PWM_on(){
	TCCR3A = (1 << COM3A0);
		// COM3A0: Toggle PB3 on compare match between counter and OCR3A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
		// WHM32: When counter (TCNT3) matches OCR3A, reset counter
		// // CS31 & CS30: Set a prescalar of 64
	set_PWM(0);
}

void PWM_off(){
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

enum States{Start,Init, Inc, Dec, Toggle, Wait}state;
double freqScales[9] = {0,261.63, 293.66,329.63,349.23,392.00,440.00,493.88,523.25}; // An array to hold the melody of the scales
unsigned char temp, tempA;
unsigned char index = 0;
void Tick_SM2(){
	switch(state){
		case Start:
			state = Init;
			break;
		case Init:
			if(tempA == 0x01){
				state = Inc;
			}
			else if(tempA == 0x02){
				state = Dec;
			}
			else if(tempA == 0x04){
				state = Toggle;
			}
			else
			{
				state = Init;
			}
			break;
		case Inc:
			state = Wait;
			break;
		case Dec:
			state = Wait;
			break;
		case Toggle:
			state = Wait;
			break;
		case Wait:
			if(tempA == 0x00)
				state = Init;
			else
				state = Wait;
			break;
		default:
			break;
	}
	switch(state){
		case Start:
			break;
		case Init:
			break;
		case Inc:
			if(index <  8){
				index++;
			}
			else{
				index = 8;
			}
			break;
		case Dec:
			if(index > 1)
				index--;
			else
				index = 1;
			break;
		case Toggle:
			if(temp){
				PWM_off();
				temp = 0;
			}
			else{
				PWM_on();
				temp=1;
			}
			break;
		case Wait:
			set_PWM(freqScales[index]);
			break;
		default:
			break;
	}
}
int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
    PWM_on();
    state = Start;
    temp = 1;
    while (1) {
	    tempA = ~PINA & 0x07;
	    Tick_SM2();
    }
    PWM_off();
    return 1;
}
