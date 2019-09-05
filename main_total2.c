// ****** Improving on colors in flicker (Working as of July 19 2019) *****

#define F_CPU 8000000UL

/* This means 8000000 cycles per second. This translates to 1/8000000 =>
   125ns, therefore 1 cycle is 125 ns */

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define numcolour 25
#define redpress (~PINB) & (_BV(1)) //PB1
#define partpress (~PINB) & (_BV(0)) //PB0


unsigned volatile int colourarray[numcolour][3] =
{
	{255,0,0}, 
	{232,12,122}, 
	{253,13,255}, 
	{167,0,232}, 
	{114,0,255}, 
	{50,12,232}, 
	{13,36,255}, 
	{0,83,232}, 
	{0,159,255}, 
	{12,207,232}, 
	{12,207,232}, 
	{13,255,216}, 
	{0,232,129}, 
	{0,255,70}, 
	{14,232,12}, 
	{100,255,13}, 
	{164,232,0}, 
	{255,251,0}, 
	{232,206,12}, 
	{255,202,13}, 
	{232,158,0}, 
	{255,147,0}, 
	{232,107,12}, 
	{255,82,13}, 
	{232,34,1},
}; 	  
unsigned volatile int adcval[3];  
unsigned volatile int  channel = 1;

unsigned volatile int redduty = 0;
unsigned volatile int greenduty = 0;
unsigned volatile int blueduty = 0; 
unsigned volatile int lightbuffer = 0;

unsigned volatile int adc_val_1 = 0;
unsigned volatile int adc_val_2 = 0;
unsigned volatile int i = 0;
unsigned volatile int tempadc = 3; 
unsigned volatile int maxcol = 250; 

unsigned volatile int redbut = 0;     
unsigned volatile int redenable = 0;
unsigned volatile int partbut = 0;
unsigned volatile int partenable = 0;

unsigned int tempred;
unsigned int tempgreen;
unsigned int tempblue;

void potenable(){ 
  //    _delay_us(1);
  //if (adcval[1] <= 5){ //red 
  //        redduty = maxcol;
  //        greenduty = maxcol - 155;
  //        blueduty = 0;
  //}
      if ( adcval[1] > 5 && adcval[1] <= 85){//red to green 
              redduty = maxcol - (adcval[1]*3);
              greenduty = maxcol - redduty;
              blueduty = 0;
     }
     else if (adcval[1] > 85 && adcval[1] <= 170){//green to blue
              greenduty = maxcol - ((adcval[1] - 85)*3);
              blueduty = maxcol - greenduty;
              redduty = 0;

     }
     else if (adcval[1] > 170 && adcval[1] <= 250){//blue to red  
              blueduty = maxcol - ((adcval[1] - 166)*3);
              redduty = maxcol - blueduty;
              greenduty = 0;
     }
     else{
              redduty = maxcol;
              greenduty = 0;
              blueduty = 0;
     }
}


//void lightpot(){  //Not using this as of now, WAS A TEST
//  //    _delay_us(1);
//     if (adcval[2] <= 30){ //red
//       lightbuffer = 30;
//     }
//     else if ( adcval[2] > 30 && adcval[2] <= 85){//red to green
//       lightbuffer = 60;
//     }
//     else if (adcval[2] > 85 && adcval[2] <= 170){//green to blue
//       lightbuffer = 90;
//     }
//     else if (adcval[2] > 170 && adcval[2] <= 240){//blue to red
//       lightbuffer = 150;
//     }
//     else{
//       lightbuffer = 250;
//     }
//}

void redlight(){
	redduty = 250; 
	greenduty = 0; 
	blueduty = 0;
	lightbuffer = 0;
} 

void partylight(){ 
        _delay_us(500); 
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
		if (i > (numcolour - 1)){
			i = 0;
		}
	}
}

//** NOTE: COMPONENTS OF RGB MODEL FOR HSV CONVERSION SHOULD HAVE VALUES IN
//** THE RANGE OF [0,1]
//** Hue should have range of [0,360]

//int hsv_mincomp(int red, int green, int blue){
//  
//  int col_arr [3] = {red, green, blue};
//  int i;
//  int min = col_arr[0]; //keeps track of which value is the smallest
//  int pos = 0; //keeps track of which RGB component is the smallest
//  for (i = 1; i < 3; i ++ ) {
//    if (min > col_arr[i]){
//      min = col_arr[i];
//      pos = i;
//    }
//  }
//  return pos;
//  
//}
//
//int hsv_maxcomp(int red, int green, int blue){
//  
//  int col_arr [3] = {red, green, blue};
//  int i;
//  int max = col_arr[0]; //keeps track of which value is the smallest
//  int pos = 0; //keeps track of which RGB component is the smallest
//  for (i = 1; i < 3; i ++ ) {
//    if (max < col_arr[i]){
//      max = col_arr[i];
//      pos = i;
//    }
//  }
//  return pos;
//  
//}
//
//int chrome(int value, int sat){
//
//  int c = value * sat;
//  return c;
//  
//}

//******* For POT 1
volatile int adc_convo_1(){
  // Make PA1 the channel of focus
  ADMUX = (ADMUX & 0b11000000) | 1;
  // Start the conversion
    ADCSRA |= (1 << ADSC);
    // Wait for it to finish
    while (!(ADCSRA & (1 << ADIF)));
    ADCSRA |= (1 << ADIF);
return ADCH;
}
//**********
//******* For POT 2
volatile int adc_convo_2(){
  // Make PA1 the channel of focus
  ADMUX = (ADMUX & 0b11000000) | 2;
  // Start the conversion
    ADCSRA |= (1 << ADSC);
    // Wait for it to finish
    while (!(ADCSRA & (1 << ADIF)));
    ADCSRA |= (1 << ADIF);
return ADCH;
}
//*********
int main (void) {
	init();
	  int temp = 0;
	while (1){
	  temp = adc_convo_2();
	adcval[1] = adc_convo_1();
	  _delay_us(10) ;
	adcval[2] = adc_convo_2();
	if (adcval[2] != temp+1 || adcval[2] != temp-1 || adcval[2] != temp + 2 || adcval[2] != temp - 2 || adcval[2] != temp) {
		lightbuffer = temp;
	  }
	  else {
	    lightbuffer = adcval[2];
	  }
	//********RED-MODE*******************
	while(redenable == 1){
	  redlight();
	  DDRA &=~ (1 << PA5);
	}
	  DDRA |= (1 << PA5);
	  potenable();
	//**************************
	//*********PARTY-MODE*****************
	if (partenable == 1){
	  DDRA &=~ (1 << PA5);
	  redduty = 0;
	  greenduty = 0;
	  blueduty = 0;
	  while (partenable){
	    partylight();
	  }
	}else{
	  DDRA |= (1 << PA5);
	  potenable();
	}
	//**************************
  } 
}

void init(){
	DDRB |=  (1 << PB2); //OCOA 
	DDRA |= (1 << PA7) | (1 << PA6) | (1 << PA5); //OCOB,//OC1A
						    ////respectively
	// test  DDRA |= (1 << PA0) | (1 << PA1) | (1 << PA2); //enabling output of led 
       //test    PORTA &=~  (1 << PA0) | (1 << PA1) | (1 << PA2);//initially off
       DDRA &=~ (1 << PA1) | (1 << PA2); //setting this pin an input kji
	TCCR0A |= ( (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00));// OCOA, OCOB, non-inverting, fast pwm 
	TIMSK0 |= (1 << TOIE0); //Timer mask for Timer/Counter 0  
	TCCR1A |= ( (1 << COM1A1) | (1 << WGM10) | (1 << WGM12) | (1 << COM1B1)); //OC1A, OC1B, fast pwm, 8bit
	TIMSK1 |= (1 << TOIE1); //Timer mask for Timer/Counter 1
	pinint();
	ADCinit();
	adcnoisecancellor();
	sei();
	TCCR0B |= (1 << CS00); //Prescaler 1 Timer 0
	TCCR1B |= (1 << CS10); //Prescaler 1 Timer 1
}

void adcnoisecancellor(){
  MCUCR |= (1 << SE) | (1 << SM0); //enabling sleep bit and adc noise cancellor
}

void pinint() {
  
DDRB &= ~ (1 << PB0) | (1 << PB1);   //enabling input on button 
PORTB |= (1 << PB0) | (1 << PB1); //tunring on internal
					 //resistor
GIMSK |= (1 << PCIE1); //enabling pin change interrupt
PCMSK1 |= (1 << PCINT9) | (1 << PCINT8); //specifying the pins that
					 //cause interrupt  (PB0 =>
					 //PCINT8) and (PB1 => PCINT9)
}
void ADCinit(){  
	ADMUX &=~ ((1 << REFS1) | (1 << REFS0));//Vcc as analog voltage
						//reference
	ADMUX |= (1 << MUX1); //Using pin ADC1 (PA1) 
	ADCSRA |=(1 << ADPS1)|(1 << ADPS0)|(1 << ADEN); // enabling only
							 // the reference
							 // and the ADC
							 // enable 
	ADCSRB |= (1 << ADLAR); //left adjust bits, since working with 8bit
				//fast pwm
	// test ADCSRA &= ~ (1 << ADEN); 
	//DIDR0 |= (1 << ADC1D);
	  /*Disables the digital buffer of the
				   analog pin being used (ADC1D), reduces
				   power consumption*/
}  
ISR(TIM0_OVF_vect){ //update dutycycle value at end of PWM cycle
	OCR0A = redduty;
	OCR0B = greenduty;
}
ISR(TIM1_OVF_vect){ //update dutycycle value at end of PWM cycle
        OCR1A = blueduty;
	OCR1B = lightbuffer;
}
ISR (PCINT1_vect){ //PCINT1 takes care of pins PCINT11:8
    _delay_us(500);
  if (redpress && redbut == 0){
    partenable = 0;
    redenable = !redenable;
    //DDRA &=~ (1 << PA5);
    redbut = 1;
  }else {
    redbut = 0;
  }
  
  if (partpress && partbut == 0){
    redenable = 0;
    partenable = !partenable;
    partbut = 1;
  }else {
    partbut = 0;
  }
}
