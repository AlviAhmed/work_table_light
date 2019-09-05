#define F_CPU 8000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>


int main (void) { 
	init();
    	while (1)  
        {
	  _delay_us(500);
	  PORTB ^= (1 << PB3);
	  PORTB |= (1 << PB4);
    	}
     }
void init(){
  DDRB |= (1 << PB3) | (1 << PB4);      
  PORTB |= (1 << PB3) | (1 << PB4);
} 

