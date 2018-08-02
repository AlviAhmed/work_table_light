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
//		while (ADCSRA & (1 << ADSC));
/*looping till convo, ends can also use an interrerupt for this instead and just the adcValue = ADC in the ISR*/
//		adcValue = ADC;
//		ledValue = (adcValue >> 2); //converting 10bit to 8bit by shifting down 2 bits

    }
} 

void init () {
    DDRA &= ~ (_BV(PA1)); //setting this pin as an input
    DDRA |= (_BV(PA5)); //the OC1B
    TCCR1A |= (1 << COM1B1) | (1 << WGM10) | (1 << WGM11); //non inverting mode, fast pwm 10-bit
    TCCR1B |= (1 << WGM12);  //10bit fast pwm
//	TIMSK1 |= (1 << TOIE1); //overflow interrupt
    sei();
    TCCR1B |= (1 << CS10); //no prescaling (1)

} 

void ADCinit() {
    ADMUX &=~ ((1 << REFS1) | (1 << REFS0));//Vcc as analog voltage reference
    ADMUX |= (1 << MUX0); //Using pin ADC1 (PA1)
    ADCSRA |= (1 << ADPS1)|(1 << ADPS2)|(1 << ADEN)|(1 << ADATE) | (1 << ADSC) | (1 << ADIE); /*Prescaler of 8, enable ADC*/
    sei();
//	ADCSRB |= (1 << ADLAR); //left adjust bits
    ADCSRB &=~ ( (1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0) );//free running mode
//	DIDR0 |= (1 << ADC1D); /*Disables the digital buffer of the analog pin being used (ADC1D),
//	reduces power consumption*/
    ADCSRA |= (1 << ADSC); //start convo

}  

ISR(ADC_vect){
    OCR1B  = ADC;
}
/*
ISR(TIM1_OVF_vect){ 

OCR1B = dutycycle;

}

*/
