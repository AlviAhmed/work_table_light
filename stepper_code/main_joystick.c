#define F_CPU 8000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define butpress (~PINB) & (_BV(1)) //PB1
unsigned volatile int but = 0;
unsigned volatile int direction = 0;

int main (void) { 
	init();
    	while (1)  
        {
	   _delay_ms(10);
	   while (direction){
	     PORTB |= (1 << PB4);
	     PORTB &=~ (1 << PB0);
	   }
	     PORTB |= (1 << PB0);
	     PORTB &=~ (1 << PB4);
	  // PORTB ^= (1 << PB2);

	  // PORTB ^= (1 << PB1);
    	}
     }
void init(){
  DDRB |= (1 << PB0) | (1 << PB4) | (1 << PB3);
  PORTB &=~ (1 << PB0) | (1 << PB4);
  PORTB &=~ (1 << PB3);
  pininit();
}

void ADC_init(){
  DDRB &=~ (1 << PB2);
  ADMUX &=~ ((1 << REFS1) | (1 << REFS0));//Vcc as analog voltage //reference
  ADMUX |= (1 << MUX1); //Using pin ADC1 (PA1) 
  ADCSRA |=(1 << ADPS1)|(1 << ADPS0)|(1 << ADEN); // enabling only
							 // the reference
							 // and the ADC
							 // enable 
  ADCSRB |= (1 << ADLAR); //left adjust bits, since working with 8bit
}

void pininit(){
 DDRB &=~ (1 << PB1); //PB1 is button and PB2 is analog
 PORTB |= (1 << PB1); //turning on internal resistor for button
GIMSK |= (1 << PCIE); //enabling pin change interrupt
PCMSK |= (1 << PCINT1); //specifying the pins that causes interrupt
  sei();
}

ISR (PCINT0_vect){ //PCINT1 takes care of pins PCINT11:8
    _delay_ms(5);
    if (butpress && but == 0){
      direction = !direction; 
      but = 1;
    }else {
      but = 0;
      }
  
}
