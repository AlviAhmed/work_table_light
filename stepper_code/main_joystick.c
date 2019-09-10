#define F_CPU 8000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define butpress (~PINB) & (_BV(1)) //PB1
#define redled PB3
#define greenled PB0
#define yellowled  PB4


unsigned volatile int but = 0;
unsigned volatile int direction = 0;



volatile int adc_convo_1(){
  // Start the conversion
    ADCSRA |= (1 << ADSC);
    // Wait for it to finish
    while (!(ADCSRA & (1 << ADIF)));
    ADCSRA |= (1 << ADIF);
return ADCH;
}

int main (void) { 
	init();
    	while (1)  
        {
	    _delay_ms(10);
	   while (direction){
	     PORTB |= (1 << redled);
	     PORTB &=~ (1 << greenled);
	   }
	     PORTB |= (1 << greenled);
	     PORTB &=~ (1 << redled);
	  // PORTB ^= (1 << PB2);

	  // PORTB ^= (1 << PB1);
    	}
     }
void init(){
  DDRB |= (1 << greenled) | (1 << redled) | (1 << yellowled);
  PORTB &=~ (1 << greenled) | (1 << redled) | (1 << yellowled);
  pininit();
  ctc_init();
}

void ADC_init(){
  DDRB &=~ (1 << PB2);
  ADMUX &=~ ((1 << REFS1) | (1 << REFS0));//Vcc as analog voltage //reference
  ADMUX |= (1 << MUX1); //Using pin ADC1 (PB2)
  ADCSRA |=(1 << ADPS1)|(1 << ADPS0)|(1 << ADEN); // enabling only
							 // the reference
							 // and the ADC
							 // enable 
  ADCSRB |= (1 << ADLAR); //left adjust bits, since working with 8bit
}

void ctc_init(){

  TCCR1 |= (1 << CTC1); //resets timer 1 after match has been found, prescaler clk/1024
  //GTCCR |= (1 << COM1B0);
  TIMSK |= (1 << OCIE1A);
  OCR1C = 10;
  TCCR1 |= (1 << CS13)|(1 << CS12)|(1 << CS11)|(1 << CS10); //prescale of 128
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

ISR (TIMER1_COMPA_vect){ //PCINT1 takes care of pins PCINT11:8
  PORTB ^= (1 << yellowled);
}
