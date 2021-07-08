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
unsigned short my_short = 0xABCD;
void ADC_init(){
        ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
        //ADEN: setting this bit enables analog to digitial conversion
        //ADSC: setting this bit starts the first conversion
        //ADATE: setthing this bit enable auto triggering. since we are
        //              in Free Running Mode, a new conversion will trigger
        //              whenever the previous conversion completes
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0xFF; PORTB = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */
    ADC_init();
    unsigned char min = 0x0F;
    while (1) { //max is 0x147
            my_short = ADC;
            if(my_short >= min)
                    PORTB = 0x01;
            if(my_short >= min *2)
                    PORTB = 0x03;
            if(my_short >= min *3)
                    PORTB = 0x07;
            if(my_short >= min * 4)
                    PORTB = 0x0F;
            if(my_short >= min * 5)
                    PORTB = 0x1F;
            if(my_short >= min *6)
                    PORTB = 0x3F;
            if(my_short >= min *7)
                    PORTB = 0x7F;
            if(my_short >= min *8)
                    PORTB = 0xFF;
            }
    return 1;
}
