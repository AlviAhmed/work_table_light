    #define F_CPU 8000000UL

    #include <avr/io.h>
    #include <stdio.h>
    #include <util/delay.h>
    #include <avr/interrupt.h>
    #define numcolour 3



    unsigned volatile int colourarray[numcolour][3] =
    {
    {255,0,0},
    {0,255,0},
    {0,0,255}
    }; 	 
    unsigned int adcval = 0; 
    unsigned int curradc = 0;
    unsigned volatile int redduty = 0;
    unsigned volatile int greenduty = 0;
    unsigned volatile int blueduty = 0;
    unsigned volatile int i = 0; 
    unsigned int tempadc = 3; 

    int main (void) {
                init(); 

                unsigned int tempred;
                unsigned int tempgreen;
                unsigned int tempblue;

		tempred = ( colourarray[i][0]);
		tempgreen = ( colourarray[i][1]);
		tempblue = ( colourarray[i][2]);     
		while (1){
			_delay_ms(1);   
			if (adcval >= 5){ //red 
				redduty = 0;  
				blueduty = 0; 
				greenduty = 0;	
					
			} 
			if (adcval > 6 && adcval < 45){ 
				redduty = 255;  
				blueduty = 0; 
				greenduty = 0;	

			}
			if ( adcval > 46 && adcval < 85){//red to green 
				redduty = 255 - (adcval*3); 
				greenduty = 255 - redduty; 
				blueduty = 0;
			} 
			if (adcval > 85 && adcval < 170){//green to blue
				redduty = 0; 
				greenduty = 255 - (adcval*3); 
				blueduty = 255 - greenduty;
						
			} 
			if (adcval > 170 && adcval < 255){//blue to red 
				redduty = 255 - blueduty; 
				greenduty = 0; 
				blueduty = 255 - (adcval*3);
			} 
			if ( adcval >= 250){ 
				redduty = 255;  
				blueduty = 0; 
				greenduty = 0;	
									
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
                sei(); 
		TCCR0B |= (1 << CS00); //Prescaler 1 Timer 0
		TCCR1B |= (1 << CS10); //Prescaler 1 Timer 1
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

