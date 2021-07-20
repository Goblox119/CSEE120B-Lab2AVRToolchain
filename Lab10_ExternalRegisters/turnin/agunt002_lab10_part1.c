/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	DEMO VIDEO: https://drive.google.com/file/d/18rlc0DNdBAMkCBmWPCq0YjFGTRRV95BD/view?usp=sharing
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

void transmit_data(unsigned char data) {
    int i;
    for (i = 0; i < 8 ; ++i) {
   	 // Sets SRCLR to 1 allowing data to be set
   	 // Also clears SRCLK in preparation of sending data
   	 PORTC = 0x08;
   	 // set SER = next bit of data to be sent.
   	 PORTC |= ((data >> i) & 0x01);
   	 // set SRCLK = 1. Rising edge shifts next bit of data into the shift register
   	 PORTC |= 0x02;
    }
    // set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
    PORTC |= 0x04;
    // clears all lines in preparation of a new transmission
    PORTC = 0x00;
}


enum States{Start, Init, IncPress, DecPress, Inc, Dec, Reset}state;
unsigned char button,cnt;
void TickSM3(){
	switch(state){
		case Start:
			state = Init;
			break;
		case Init:
			if(button == 0x01){
				state = IncPress;
			}
			else if(button == 0x02){
				state = DecPress;
			}
			else if(button == 0x03){
				state = Reset;
			}
			else if(button == 0x00){
				state = Init;
			}
			break;
		case IncPress:
			if(button == 0x01){
				state = Inc;
			}
			else if(button == 0x02){
				state = DecPress;
			}
			else if(button == 0x03){
				state = Reset;
			}
			else if(button == 0x00){
				state = Init;
			}
			break;
		case DecPress:
			if(button == 0x01){
				state = IncPress;
			}
			else if(button == 0x02){
				state = Dec;
			}
			else if(button == 0x03){
				state = Reset;
			}
			else if(button == 0x00){
				state = Init;
			}
			break;
		case Inc:
			if(button == 0x01){
				state = Inc;
			}
			else if(button == 0x02){
				state = DecPress;
			}
			else if(button == 0x03){
				state = Reset;
			}
			else if(button == 0x00){
				state = Init;
			}
			break;
		case Dec:
			if(button == 0x01){
				state = IncPress;
			}
			else if(button == 0x02){
				state = Dec;
			}
			else if(button == 0x03){
				state = Reset;
			}
			else if(button == 0x00){
				state = Init;
			}
			break;
		case Reset:
			if(button == 0x01){
				state = IncPress;
			}
			else if(button == 0x02){
				state = DecPress;
			}
			else if(button == 0x03){
				state = Reset;
			}
			else if(button == 0x00){
				state = Init;
			}
			break;
	}
	switch(state){
		case Start:
			tempc = 0x00;
			cnt = 0x00;
			break;
		case Init:
			cnt = 0x00;
			break;
		case IncPress:
			cnt = 0x00;
			if(tempc < 0xFF){
				tempc += 0x01;
			}
			break;
		case DecPress:
			cnt = 0x00;
			if(tempc > 0x00){
				tempc -= 0x01;
			}
			break;
		case Inc:
			if((tempc < 0xFF) && (cnt >= 0x0A)) {
					tempc += 0x01;
					cnt = 0x00;
			}
			cnt += 0x01;
			break;
		case Dec:
			if((tempc > 0x00) && (cnt >= 0x0A)) {
                                        tempc -= 0x01;
					cnt = 0x00;
                        }
			cnt += 0x01;
                        break;
		case Reset:
			cnt = 0x00;
			tempc = 0x00;
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    
    TimerSet(100);
    TimerOn();
    state = Init;

    while(1) {
	    button = ~PINA & 0x03;
	    while(!TimerFlag);
	    TimerFlag = 0;
	    TickSM3();
	    transmit_data(tempc);
    }
    return 1;
}
