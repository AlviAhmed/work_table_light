/* ATTINY 84*/

#define F_CPU 1000000UL  
#include <avr/io.h> 
#include <stdio.h> 
#include <util/delay.h> 
#include <avr/interrupt.h>

int main (void){
	init();
	while (1){ 

	
	}  

} 

void init () { 
	DDRA &= ~ (_BV(PA0)); //setting this pin as an input 
	DDRB |= (_BV(PB0)) | (_BV(PB1)); //two output leds to test brightness scrolling 
	
}
