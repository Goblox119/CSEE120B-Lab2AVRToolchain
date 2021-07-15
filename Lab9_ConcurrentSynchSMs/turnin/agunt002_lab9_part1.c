/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	DEMO VIDEO: https://drive.google.com/file/d/1r1Qgy2W7__TN4wxWW53_B7Hkxo_GJuhm/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum TLEDStates{Start, L0, L1, L2}TLEDstate;
enum BLEDStates{Bstart, Off, On}BLEDstate;
enum CLEDStates{Cstart, Combine}CLEDstate;
unsigned char tempt = 0x00;
unsigned char tempb = 0x00;

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

void TLED_Tick(){
	switch(TLEDstate){
		case Start:
			TLEDstate = L0;
			break;
		case L0:
			TLEDstate = L1;
			break;
		case L1:
			TLEDstate = L2;
			break;
		case L2:
			TLEDstate = L0;
			break;
	}
	switch(TLEDstate){
		case Start:
			break;
		case L0:
			tempt = 0x01;
			break;
		case L1:
			tempt = 0x02;
			break;
		case L2:
			tempt = 0x04;
			break;
	}
}
void BLED_Tick(){
	switch(BLEDstate){
		case Bstart:
			BLEDstate = On;
			break;
		case On:
			BLEDstate = Off;
			break;
		case Off:
			BLEDstate = On;
			break;
	}
	switch(BLEDstate){
		case Bstart:
			break;
		case On:
			tempb = 0x08;
			break;
		case Off:
			tempb = 0x00;
			break;
	}
}

void CLED_Tick(){
	switch(CLEDstate){
		case Cstart:
			CLEDstate = Combine;
			break;
		case Combine:
			CLEDstate = Combine;
			break;
	}
	switch(CLEDstate){
		case Cstart:
			break;
		case Combine:
			PORTB = (tempt |tempb);
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
    TLEDstate = Start;
    BLEDstate = Bstart;
    CLEDstate = Cstart;
    TimerSet(1000);
    TimerOn();
    while (1) {
	    TLED_Tick();
	    BLED_Tick();
	    CLED_Tick();
	    while(!TimerFlag);
	    TimerFlag = 0;

    }
    return 1;
}
