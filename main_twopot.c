//Working with 2 potentiometers 
#define F_CPU 8000000UL 

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>  
#include <avr/interrupt.h> 

unsigned volatile int adcval[3]; 
unsigned volatile int channel = 1;
unsigned volatile int redduty = 0;
unsigned volatile int greenduty = 0;
unsigned volatile int blueduty = 0;
unsigned volatile int i = 0;
unsigned int maxcol = 255;

int main (void){ 
        init();

        while (1){   		    
	
	} 
        
} 

void init(){ 
	DDRB |= (1 << PB2);
	DDRA |= (1 << PA6) | (1 << PA5);// OC1A, OC1B respectively  
	TCCR0A |= ( (1 << COM0A1)  | (1 << WGM01) | (1 << WGM00));// OCOA, OCOB, non-inverting, fast pwm  
	TIMSK0 |= (1 << TOIE0); //Timer mask for Timer/Counter 0  
	TCCR1A |= ( (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10) | (1 << WGM12) ); //OC1A, non-inverting, fast 8-bit pwm, for blue pin and OC1B respectively
	TIMSK1 |= (1 << TOIE1); //Timer mask for Timer/Counter 1  
	ADCinit();
	sei(); 
	TCCR0B |= (1 << CS00); //Prescaler 1 Timer 0
	TCCR1B |= (1 << CS10); //Prescaler 1 Timer 1
}

void ADCinit(){
	DDRA &=~ (1 << PA1) | (1 << PA2); //setting this pin an input for pot 
	ADMUX &=~ ((1 << REFS1) | (1 << REFS0));//Vcc as analog voltage reference
	ADMUX |= (1 << MUX0); //Using pin ADC1 (PA1) 
	ADCSRA |= (1 << ADPS1)|(1 << ADPS2)|(1 << ADEN)|(1 << ADATE) | (1 << ADIE); /*Prescaler of 64, enable ADC*/
	ADCSRB |= (1 << ADLAR); //left adjust bits, since working with 8bit fast pwm
	DIDR0 |= (1 << ADC1D); /*Disables the digital buffer of the analog pin being used (ADC1D), reduces power consumption*/
	startconvo();
}

void startconvo(){ 
	ADCSRA |= (1 << ADSC); //start convo
} 

ISR(ADC_vect){ 
	adcval[channel] = ADCH;        
	channel ++;  
	if (channel > 2){ 
		channel = 1;	
	}  
	ADMUX = (ADMUX & 0b11000000) | channel;           
	
	
}

ISR(TIM0_OVF_vect){ 
//	OCR0A = adcval[1];
}
ISR(TIM1_OVF_vect){ //update dutycycle value at end of PWM cycle   
	OCR1A = adcval[1]; //PA6
	OCR1B = adcval[2]; //PA5
}



