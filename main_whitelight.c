// *** main_whitelight.c ***

//*****clock fuses so that 8Mhz

#define F_CPU 8000000UL
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define ledprt PA7

unsigned volatile int lightbuffer;


void startconvo() {
  /*  */
  ADCSRA |= (1 << ADSC); //start ADC conversation
  
}

int main (void){
  init();
  while(1){
    
    //    _delay_ms(1000);
    //    PORTA ^= (1 << PA7);
    
  }
  return (0);
}

void init(){
  
  DDRA &=~ (1 << PA1); //Making PA1 an input for pot
  DDRA |= (1 << PA7); //this pin is OC0B
  //  PORTA |= (1 << PA7);
  TIMSK0 |= (1 << TOIE0); //Timer mask for Timer/Counter 0
  TCCR0A |= ( (1 << COM0B1) | (1 << WGM01) | (1 << WGM00));//  OCOB, non-inverting, fast
									   // pwm
  
  ADCinit();
  sei();
  
  /* fast pwm calculations focnx = fclk / (N*256)  
starting off with a large prescale just to see if significant effect
     i.e. focnx = 8000000/ (256*256) => 122Hz
*/
  TCCR0B |= (1 << CS02); //tried lowering the prescale to see what a faster freq pwm does to the led light
			 //strip, it just turned off earlier, and flickers through the high end of the ADC
			 //values.
  
}

void ADCinit(){
  ADMUX &=~ (1 << REFS1) | (1 << REFS0); //setting REFS0 and REFS1 to zero to ensure VCC as analog voltage
				     //reference 
  ADMUX |= (1 << MUX0); //Setting pin PA1 (ADC1) to be the potentiometer 
  ADCSRA |= (1 << ADEN); //enable the ADC
  ADCSRA |= (1 << ADIE); //enabling ADC interrupt
  ADCSRA |= (1 << ADATE); //enable ADC auto trigger
  //for the ADC clock, system clock = 8mHz, recommended between 50Khz and 200 Khz, will need to see the
  //difference between making the ADC 50 kHz and 200 kHz

  //8mHz / 200kHz => 40, next largest is 64 so will go with that.
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1); //using prescaler of 64
  //ADC clock will therefore be 125 kHz
  ADCSRB |= (1 << ADLAR); //enabling ADLAR for left shifting of adc registers so that I can only read the high
			  //byte

  // Tried increasing and decreasing the prescaler, not much difference, seems that the leds are flickers when
  // the potentiometer is touched, may have to find a way to "debounce" a potentiometer 
  ADCSRB &= ~ (1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0); //making sure they are off for free running mode
  
  startconvo();
  
}
// It is recommended that when changing ADC channels the ADC should be forced into a long conversation this can
// be by turning off the ADC then changing the multiplexer settings and then turning on the ADC 

ISR (ADC_vect){

  lightbuffer = ADCH; //using only ADCH since left shifted

}

ISR (TIM0_OVF_vect) {

	OCR0B = lightbuffer;
	
}
