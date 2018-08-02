    #define F_CPU 8000000UL

    #include <avr/io.h>
    #include <stdio.h>
    #include <util/delay.h>
    #include <avr/interrupt.h>
    #define numcolour 3



    unsigned volatile int colourarray[numcolour][3] =
    {
    {250,0,0},
    {0,250,0},
    {0,0,250}
    };
    unsigned int adc;
    unsigned volatile int redduty = 0;

    unsigned volatile int greenduty = 0;

    unsigned volatile int blueduty = 0;


    unsigned volatile int i = 0;

    int main (void) {
                init();

                unsigned int tempred;
                unsigned int tempgreen;
                unsigned int tempblue;


                while (1){
			_delay_ms(15);

			tempred = ( colourarray[i][0]);
			tempgreen = ( colourarray[i][1]);
			tempblue = ( colourarray[i][2]);

			if(redduty < tempred){redduty ++ ;}
			if(redduty > tempred){redduty -- ;}


                        if (greenduty < tempgreen){greenduty ++;}
                        if(greenduty > tempgreen) {greenduty --;}

                        if (blueduty < tempblue){blueduty ++;}
                        if(blueduty > tempblue) {blueduty --;}


                if ((redduty == tempred) && (greenduty == tempgreen) && (blueduty == tempblue)){
                        i++;
                        if (i > numcolour){
                                i = 0;
                        }
                }



                }
         }






    void init(){
        	DDRB |=  (1 << PB2); //OCOA 
		DDRA |= (1 << PA7) | (1 << PA6);//OCOB, OC1A respectively 

                TCCR0A |= ( (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00));// OCOA, OCOB, non-inverting, fast pwm 
		TIMSK0 |= (1 << TOIE0); //Timer mask for Timer/Counter 0  

		TCCR1A |= (1 << COM1A1 | (1 << WGM10) | (1 << WGM12)); //OC1A, non-inverting, fast 8-bit pwm, for blue pin 
		TIMSK1 |= (1 << TOIE1); //Timer mask for Timer/Counter 1 

                sei(); 

		TCCR0B |= (1 << CS00); //Prescaler 1 Timer 0
		TCCR1B |= (1 << CS10); //Prescaler 1 Timer 1


    }



         ISR(TIM0_OVF_vect){ //update dutycycle value at end of PWM cycle
	        OCR0A = redduty;
                OCR0B = greenduty;
        }

        ISR(TIM1_OVF_vect){ //update dutycycle value at end of PWM cycle 
                OCR1A = blueduty;
        }

~          
