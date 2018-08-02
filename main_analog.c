/* ATTINY 84*/

#define F_CPU 8000000UL  
#include <avr/io.h> 
#include <stdio.h> 
#include <util/delay.h> 
#include <avr/interrupt.h>  


double  dutycycle;
uint8_t ledValue = 0; 
uint16_t adcValue = 0;  
unsigned int i = 0; 

int main (void){
	init(); 
	while (1){     
	
	}  
}  


void init () { 
	DDRA &= ~ (_BV(PA1)); //setting this pin as an input 
	DDRA |= (_BV(PA5)); //the OC1B   
	TCCR1A |= (1 << COM1B1) | (1 << WGM10) | (1 << WGM11); //non inverting mode, fast pwm 10-bit  
	TCCR1B |= (1 << WGM12);  //10bit fast pwm
	TIMSK1 |= (1 << TOIE1); //overflow interrupt  
	ADCinit();
	sei(); 
	TCCR1B |= (1 << CS10) ; // prescaler (1)
	
} 

void ADCinit() { 
	ADMUX &= ~ (1 << REFS1)|(1 << REFS0);//Vcc as analog voltage reference 
	ADMUX |= (1 << MUX0); //Using pin ADC1 (PA1)
	ADCSRA |= (1 << ADPS1)|(1 << ADPS2)|(1 << ADEN)|(1 << ADIE); /*Prescaler of 64, enable ADC*/      
	DIDR0 |= (1 << ADC1D); 
	startconvo();
}   
void startconvo (){ 
	ADCSRA |= (1 << ADSC);
}

ISR(TIM1_OVF_vect){ 
	OCR1B = dutycycle;
} 

ISR(ADC_vect){    
	dutycycle  = ADC;  
	startconvo();
	
}   



