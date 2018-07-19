/* ATTINY 84*/

#define F_CPU 1000000UL  
#include <avr/io.h> 
#include <stdio.h> 
#include <util/delay.h> 
#include <avr/interrupt.h> 

int dutycycle;

int main (void){
	init(); 
	ADCinit();
	while (1){ 
		ADCSRA |= (1 << ADSC); //enabling the adc 
		loop_until_bit_is_clear(ADCSRA, ADSC); //wait until convo is done 
		adcValue = ADC; 
		ledValue = (adcValue >> 2); //converting 10bit to 8bit by shifting down 2 bits
		dutycycle = ledValue;
	}  
} 

void init () { 
	DDRA &= ~ (_BV(PA1)); //setting this pin as an input 
	DDRB |= (_BV(PA5)); //the OC1B  
	uint8_t ledValue; 
	uint16_t adcValue;  
	TCCR1A |= (1 << COM1B1) | (1 << WGM10); //non inverting mode, fast pwm 8-bit  
	TCCR1B |= (1 << WGM12);  
	TIMSK1 |= (1 << TOIE1);
	sei(); 
	TCCR1B |= (1 << CS10); //no prescaler
	
} 

void ADCinit() { 
	 //internal voltage reference of 1.1V, no voltage to AD0
	ADCSRA |= (1 << ADPS1)/*prescaler of 4*/ | (1 << ADEN); /*Enable ADC*/ 

} 


