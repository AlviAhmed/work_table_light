/* ATTINY 84*/

#define F_CPU 8000000UL  
#include <avr/io.h> 
#include <stdio.h> 
#include <util/delay.h> 
#include <avr/interrupt.h>  



int dutycycle;
uint8_t ledValue; 
uint16_t adcValue; 

int main (void){
	init(); 
	ADCinit();
	while (1){  
		ADCSRA |= (1 << ADSC); //enabling the adc  
		while (ADCSRA & (1 << ADSC)); /*looping till convo, ends 
		can also use an interrerupt for this instead and just the adcValue = ADC in the ISR*/
		adcValue = ADC; 
		ledValue = (adcValue >> 2); //converting 10bit to 8bit by shifting down 2 bits
		dutycycle = ledValue; 
		
	}  
} 

void init () { 
	DDRA &= ~ (_BV(PA1)); //setting this pin as an input 
	DDRA |= (_BV(PA5)); //the OC1B  
	TCCR1A |= (1 << COM1B1) | (1 << WGM10); //non inverting mode, fast pwm 8-bit  
	TCCR1B |= (1 << WGM12);  //8bit fast pwm
	TIMSK1 |= (1 << TOIE1); //overflow interrupt
	sei(); 
	TCCR1B |= (1 << CS10); //no prescaler (1)
	
} 

void ADCinit() { 
	ADMUX &=~ ((1 << REFS1) | (1 << REFS0));//Vcc as analog voltage reference 
	ADMUX |= (1 << MUX1); //Using pin ADC1 (PA1)
	ADCSRA |= (1 << ADPS1)|(1 << ADPS2)|(1 << ADEN); /*Prescaler of 8, enable ADC*/   
//	ADCSRB &=~ ( (1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0) );//free running mode
	DIDR0 |= (1 << ADC1D); /*Disables the digital buffer of the analog pin being used (ADC1D), 
	reduces power consumption*/

}  

ISR(TIM1_OVF_vect){ 

OCR1B = dutycycle;

}


