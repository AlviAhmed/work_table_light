//GETTING COLOURS TO WORK WITH POT
#define F_CPU 8000000UL

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define numcolour 3
#define redpress (~PINB) & (_BV(0)) //PB0 
#define partpress (~PINB) & (_BV(1)) //PB1 


unsigned volatile int colourarray[numcolour][3] =
{
{250,0,0},
{0,250,0},
{0,0,250}
}; 	  

unsigned int adcval = 0; 
unsigned int curradc = 0;
unsigned volatile int redduty = 0;
unsigned volatile int greenduty = 0;
unsigned volatile int blueduty = 0;
unsigned volatile int i = 0; 
unsigned int tempadc = 3; 
unsigned int maxcol = 255; 


int redbut = 0;     
int redenable = 0;
int partbut = 0;
int partenable = 0;

unsigned int tempred;
unsigned int tempgreen;
unsigned int tempblue;



void potenable(){  
	       _delay_ms(1);   
	       if (adcval <= 30){ //red 
		       redduty = maxcol;  
		       greenduty = maxcol - 155;	 
		       blueduty = 0; 
	       } 
	       if ( adcval > 30 && adcval <= 85){//red to green 
			redduty = maxcol - (adcval*3); 
			greenduty = maxcol - redduty; 
			blueduty = 0;
	       } 
	       if (adcval > 85 && adcval <= 170){//green to blue
			greenduty = maxcol - (adcval*3); 
			blueduty = maxcol - greenduty;
			redduty = 0; 
	       			
	       } 
	       if (adcval > 170 && adcval <= 240){//blue to red  
			blueduty = maxcol - (adcval*3);
			redduty = maxcol - blueduty; 
			greenduty = 0; 
	       }  
	       if (adcval >= 250){ 
			redduty = maxcol; 
			blueduty = 0; 
			greenduty = 0;			
               }
	
} 

void redlight(){ 
	redduty = 255; 
	greenduty = 0; 
	blueduty = 0;
} 

void partylight(){ 
	_delay_us(10); 
	tempred = ( colourarray[i][0]);
	tempgreen = ( colourarray[i][1]);
	tempblue = ( colourarray[i][2]);       

	if(redduty < tempred) {redduty ++ ;}
	if(redduty > tempred){redduty --;}

	if (greenduty < tempgreen){greenduty ++;}
	if(greenduty > tempgreen) {greenduty --;}

	if (blueduty < tempblue){blueduty ++;}
	if(blueduty > tempblue){blueduty --;}

	if ((redduty == tempred) && (greenduty == tempgreen) && (blueduty == tempblue)){
		i++;
		if (i > numcolour){
			i = 0;
		}
	}
}



int main (void) {
	init(); 


	while (1){ 
		if (partenable == 1){  
			redduty = 0; 
			greenduty = 0; 
			blueduty = 0; 
			while (partenable){
				partylight(); 
			}
                } 
                if (redenable == 1){
			redlight();
                }
                if (partenable != 1 && redenable != 1){ 
			potenable();
                }
                

	}			 
}






void init(){
     	DDRB |=  (1 << PB2); //OCOA 
     	DDRA |= (1 << PA7) | (1 << PA6);//OCOB, OC1A respectively 
        TCCR0A |= ( (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00));// OCOA, OCOB, non-inverting, fast pwm 
     	TIMSK0 |= (1 << TOIE0); //Timer mask for Timer/Counter 0  
     	TCCR1A |= ( (1 << COM1A1) | (1 << WGM10) | (1 << WGM12) ); //OC1A, non-inverting, fast 8-bit pwm, for blue pin 
     	TIMSK1 |= (1 << TOIE1); //Timer mask for Timer/Counter 1  
     	ADCinit();  
	pinint();
        sei(); 
     	TCCR0B |= (1 << CS00); //Prescaler 1 Timer 0
     	TCCR1B |= (1 << CS10); //Prescaler 1 Timer 1
}  

void pinint() { 
	DDRB &= ~ (1 << PB0) | (1 << PB1);   //enabling input on button 
	PORTB |= (1 << PB0) | (1 << PB1); //tunring on internal resistor
        GIMSK |= (1 << PCIE1); //enabling pin change interrupt
        PCMSK1 |= (1 << PB0) | (1 << PB1); //specifying the pins that cause interrupt 
}


void ADCinit(){  
          DDRA &=~ (1 << PA1); //setting this pin an input for pot 
          ADMUX &=~ ((1 << REFS1) | (1 << REFS0));//Vcc as analog voltage reference
          ADMUX |= (1 << MUX0); //Using pin ADC1 (PA1) 
          ADCSRA |= (1 << ADPS1)|(1 << ADPS2)|(1 << ADEN)|(1 << ADATE) | (1 << ADSC) | (1 << ADIE); /*Prescaler of 64, enable ADC*/
          ADCSRB |= (1 << ADLAR); //left adjust bits, since working with 8bit fast pwm
          DIDR0 |= (1 << ADC1D); /*Disables the digital buffer of the analog pin being used (ADC1D), reduces power consumption*/ 
          startconvo();
}  

void startconvo(){ 
      ADCSRA |= (1 << ADSC); //start convo
}   


ISR(ADC_vect){ 
	adcval  = ADCH;  
}
ISR(TIM0_OVF_vect){ //update dutycycle value at end of PWM cycle
	OCR0A = redduty;
	OCR0B = greenduty;
}
ISR(TIM1_OVF_vect){ //update dutycycle value at end of PWM cycle 
        OCR1A = blueduty;
}
ISR (PCINT1_vect){ //PCINT1 takes care of pins PCINT11:8    
	       _delay_ms(5);
        if (partpress &&  partbut == 0 ){
                redenable = 0;
                partenable = !partenable;//toggling value instead of LED 
                partbut = 1;
        }
        else {
                partbut = 0;

        }
        if (redpress && redbut == 0){
                partenable = 0;
                redenable = !redenable;
                redbut = 1;
        }
        else{
                redbut = 0;
        }
        if (partpress && redpress && partbut == 0 && redbut == 0){//if both buttons pressed, default to one of them  
                redenable = 0;
                partenable = !partenable;
                partbut = 1;
        }else {
                partbut = 0;
        }

}

