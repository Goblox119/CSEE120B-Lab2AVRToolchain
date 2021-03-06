/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	VIDEO DEMO: https://drive.google.com/file/d/1FDNyAn13TB-6EgAG5UY9v_davRo1lgPC/view?usp=sharing
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
enum ThreeLEDStates {ThreeStart, Zero, One, Two} ThreeLEDstate;
enum BlinkingLEDStates {BlinkingStart, BlinkingInit, Blink} BlinkingLEDstate;
enum CombineLEDStates {CombineStart, CombineInit} CombineLEDstate;
enum SpeakerStates {SpeakerStart, SpeakerOff, SpeakerOn, SpeakerUp, SpeakerDown, SpeakerRelease} Speakerstate;
unsigned char threeLEDs = 0x00;
unsigned char blinkingLED = 0x00;
unsigned char Speaker = 0x00;
unsigned char i = 0;
unsigned char j = 0;
unsigned char array[] = {1, 2, 3, 4, 5, 6};

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

void TickThreeLEDsSM() {
	switch(ThreeLEDstate) {
		case ThreeStart:	ThreeLEDstate = Zero; break;
		
		case Zero:	ThreeLEDstate = One;
				break;

		case One:	ThreeLEDstate = Two;
				break;

		case Two:	ThreeLEDstate = Zero;
				break;

		default:	ThreeLEDstate = ThreeStart; break;
	}

	switch (ThreeLEDstate) {
		case ThreeStart:        break;
		case Zero:		threeLEDs = 0x01; break;
		case One:		threeLEDs = 0x02; break;
		case Two:		threeLEDs = 0x04; break;
                default:        	break;
	}
}

void TickBlinkingLEDSM() {
	switch (BlinkingLEDstate) {
                case BlinkingStart:     BlinkingLEDstate = BlinkingInit; break;
                case BlinkingInit:      BlinkingLEDstate = Blink; break;
                case Blink:     	BlinkingLEDstate = BlinkingInit; break;
                default:        	BlinkingLEDstate = BlinkingStart; break;
        }
	switch (BlinkingLEDstate) {
                case BlinkingStart:     break;
                case BlinkingInit:      blinkingLED = 0x00; break;
                case Blink:    		blinkingLED = 0x01; break;
                default:        	break;
        }
}

void TickCombineLEDsSM() {
        switch (CombineLEDstate) {
                case CombineStart:      CombineLEDstate = CombineInit; break;
                case CombineInit:       CombineLEDstate = CombineInit; break;
                default:		CombineLEDstate = CombineStart; break;
        }
        switch (CombineLEDstate) {
                case CombineStart:      break;
                case CombineInit:       PORTB = ((Speaker << 4) | (blinkingLED << 3) | (threeLEDs)); break;
                default:       		break;
        }
}

void TickSpeakerSM() {
        switch (Speakerstate) {
                case SpeakerStart:      Speakerstate = SpeakerOff; break;
                case SpeakerOff:        if ((~PINA & 0x07) == 0x04) {
						Speakerstate = SpeakerOn;
					}
					else if ((~PINA & 0x07) == 0x01) {
						Speakerstate = SpeakerUp;
					}
					else if ((~PINA & 0x07) == 0x02) {
                                                Speakerstate = SpeakerDown;
                                        }
					else {
						Speakerstate = SpeakerOff;
					}	
					break;

		case SpeakerOn:		if ((~PINA & 0x07) == 0x00) {
						Speakerstate = SpeakerOff;
					}
					else {
						Speakerstate = SpeakerOn;
					}
					break;

		case SpeakerDown:	Speakerstate = SpeakerRelease;
					break;

		case SpeakerUp:		Speakerstate = SpeakerRelease;
					break;

		case SpeakerRelease:	if ((~PINA & 0x07) == 0x00) {Speakerstate = SpeakerOff;}
					else {Speakerstate = SpeakerRelease;}
					break;

                default:                Speakerstate = SpeakerStart; break;
        }
        switch (Speakerstate) {
                case SpeakerStart:      break;

                case SpeakerOff:        Speaker = 0x00; i = 0; break;

		case SpeakerOn:		if (i <= array[j]) {Speaker = 0x01;}
					else if (i <= (array[j] * 2)) {Speaker = 0x00;}
				        else {i = 0;}
					++i;
					break;

		case SpeakerUp:		if (j > 0) {--j;}
					break;

		case SpeakerDown:	if (j < 6) {++j;}
					break;

                default:                break;
        }
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    unsigned long ThreeElapsed = 0;
    unsigned long BlinkingElapsed = 0;
    const unsigned long timerPeriod = 1;
    TimerSet(1);
    TimerOn();
    ThreeLEDstate = ThreeStart;
    BlinkingLEDstate = BlinkingStart;
    CombineLEDstate = CombineStart;
    Speakerstate = SpeakerStart;

    while (1) {
	if (ThreeElapsed >= 300) {
		TickThreeLEDsSM();
		ThreeElapsed = 0;
	}
	if (BlinkingElapsed >= 1000) {
		TickBlinkingLEDSM();
		BlinkingElapsed = 0;
	}
	TickSpeakerSM();
	TickCombineLEDsSM();
	while(!TimerFlag) {};
	TimerFlag = 0;
	ThreeElapsed += timerPeriod;
	BlinkingElapsed += timerPeriod;
	
    }
    return 0;
}
