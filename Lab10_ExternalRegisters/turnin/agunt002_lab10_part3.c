/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	DEMO VIDEO: https://drive.google.com/file/d/1cucLqadVqKKPyltvypjSA6FKVICyxhd5/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char tempc = 0x00;
volatile unsigned char TimerFlag = 0;
void TimerISR() { TimerFlag = 1;}
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;
void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}
void TimerOff() {
	TCCR1B = 0x00;
}
ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}
void TimerSet (unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void transmit_data(unsigned char data, unsigned char reg) {
    int i;
    for (i = 0; i < 8 ; ++i) {
   	 // Sets SRCLR to 1 allowing data to be set
   	 // Also clears SRCLK in preparation of sending data
   	 if (reg == 1) {PORTC = 0x08;} else {PORTD = 0x08;}
   	 // set SER = next bit of data to be sent.
   	 if (reg == 1) {PORTC |= ((data >> i) & 0x01);} else {PORTD |= ((data >> i) & 0x01);}
   	 // set SRCLK = 1. Rising edge shifts next bit of data into the shift register
   	 if (reg == 1) {PORTC |= 0x02;} else {PORTD |= 0x02;}
    }
    // set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
    if (reg == 1) {PORTC |= 0x04;} else {PORTD |= 0x04;}
    // clears all lines in preparation of a new transmission
    if (reg == 1) {PORTC = 0x00;} else {PORTD = 0x00;}
}

unsigned array[8] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F};
unsigned char i = 0x00;
unsigned char j = 0x00;
unsigned char invert = 0x00;
unsigned char invert2 = 0x00;
unsigned char button = 0x00;
unsigned char go = 0x01;
enum States {Start, Begin, Init, Reset, ResetOn, Plus, Minus, PlusOn, MinusOn} state;
void Tick() {
	switch(state) {
		case Start:
			state = Begin;
			break;
		case Begin:
			state = Init;
			break;
		case Init:
			if  (((~PINA & 0x0F) == 0x03) ) {
				go = 0x01;
				state = ResetOn;
			}
			else if ((~PINA & 0x0F) == 0x02) {
				if ((invert & 0x01) == 0x00) {
					go = 0x01;
					state = MinusOn;
				}
			}
			else if ((~PINA & 0x0F) == 0x01) {
				if ((invert & 0x01) == 0x00) {
					go = 0x01;
					state = PlusOn;
				}
			}
			else if ((~PINA & 0x0F) == 0x04) {
				if ((invert2 & 0x01) == 0x00) {
					go = 0x02;
					state = PlusOn;
				}
			}
			else if ((~PINA & 0x0F) == 0x08) {
				if ((invert2 & 0x01) == 0x00) {
					go = 0x02;
					state = MinusOn;
				}
			}
			else if ((~PINA & 0x0F) == 0x0C) {
				go = 0x02;
				state = ResetOn;
			}
			else {
                        	state = Init;
                        }
			break;
		case Reset:
			if ((~PINA & 0x0F) == 0x03) {
				go = 0x01;
                                state = Reset;
                        }
			else if ((~PINA & 0x0F) == 0x0C) {
				go = 0x02;
				state = Reset;
			}
                        else {
                                state = Begin;
                        }
                        break;
		case ResetOn:
			state = Reset;
			break;
		case Plus:
			if ((~PINA & 0x0F) == 0x01) {
				if ((invert & 0x01) == 0x00) {
					go = 0x01;
                                	state = Plus;
				}
                        }
			else if ((~PINA & 0x0F) == 0x04) {
				if ((invert2 & 0x01) == 0x00) {
					go = 0x02;
					state = Plus;
				}
			}
                        else {
                                state = Init;
                        }
                        break;
		case PlusOn:
			state = Plus;
			break;
			
		case Minus:
                        if ((~PINA & 0x0F) == 0x02) {
				if ((invert & 0x01) == 0x00) {
					go = 0x01;
                                	state = Minus;
				}
                        }
			else if ((~PINA & 0x0F) == 0x08) {
				if ((invert2 & 0x01) == 0x00) {
					go = 0x02;
					state = Minus;
				}
			}
                        else {
                                state = Init;
                        }
                        break;
		case MinusOn:
			state = Minus;
			break;
			
		default:
			state = Start;
			break;
	}
	switch(state) {
		case Start:
			tempc = 0x00;
			break;
		case Begin:
			tempc = 0x00;
			break;
		case Init:
			break;
		case Plus:
			break;
		case Minus:
			break;
		case PlusOn:
			if ((go & 0x03) == 0x01) {
				++i;
				if (i > 8) {i = 8;}
				tempc = array[i];
			}
			else {
				++j;
				if (j > 8) {j = 8;}
				tempc = array[j];
			}
                        break;
		case MinusOn:
			if ((go & 0x03) == 0x01) {
				--i;
				if (i < 0) {i = 0;}
				tempc = array[i];
			}
			else {
				--j;
				if (j < 0) {j = 0;}
				tempc = array[j];
			}
                        break;
		case Reset:
			break;
		case ResetOn:
			tempc = 0x00;
			if (((go & 0x03) == 0x01) && ((invert & 0x01) == 0x01)) {
				invert = 0x00;
				i = 0x00;
			}
			else if (((go & 0x03) == 0x02) && ((invert2 & 0x01) == 0x01)) {
				invert2 = 0x00;
				j = 0x00;
			}
			else if (((invert2 & 0x01) == 0x00) && ((go & 0x03) == 0x02)) {
				invert2 = 0x01;
				j = 0x00;
			}
			else if (((invert & 0x01) == 0x00) && ((go & 0x03) == 0x01)) {
				invert = 0x01;
				i = 0x00;
			}
			break;
		default:
			tempc = 0x00;
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
    /* Insert your solution below */
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;

    TimerSet(100);
    TimerOn();
    state = Start;

    while (1) {
	button = ~PINA & 0x0F;
	while(!TimerFlag);
	TimerFlag = 0;
	Tick();
	transmit_data(tempc, go);
    }
    return 1;
}
