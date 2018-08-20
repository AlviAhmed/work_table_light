//GETTING COLOR MODES TO WORK
#define F_CPU 8000000UL 
#include <stdio.h> 
#include <avr/io.h>  
#include <util/delay.h>
#include <avr/interrupt.h> 


unsigned volatile int colourarray[numcolour][3] =
    {
    {255,0,0},
    {0,255,0},
    {0,0,255}
    };
unsigned volatile int adcval = 0;
unsigned volatile int curradc = 0;
unsigned volatile int redduty = 0;
unsigned volatile int greenduty = 0;
unsigned volatile int blueduty = 0;
unsigned volatile int i = 0;
unsigned volatile int tempadc = 3;
unsigned volatile int tempred = colourarray[i][0];
unsigned volatile int tempgreen = colourarray[i][1];
unsigned volatile int tempblue = colourarray[i][2];

int main (void) { 
	init();   
	pinint();
	while (1){
		_delay_ms(1); 
		while (cho == 1){ 
			choice();
		}  
		while (fad == 1){ 
			fade();	
		}
       }

}


void choice () {  
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

void fade () {  

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

void pinint() {
        GIMSK |= (1 << PCIE1); //enabling pin change interrupt
        PCMSK1 |= (1 << PB0) | (1 << PB1); //specifying the pins that cause interrupt 
        sei();
}

ISR (PCINT1_vect){ //PCINT1 takes care of pins PCINT11:8 
        _delay_ms(5);
        if (partpress && (partbut == 0)){//turning on first led 
                PORTA ^= (1 << PA2); //turns on led
                partbut = 1;
        }
        else{
                partbut = 0;
        }

        if (colpress && (colbut == 0)){//turning on first led 
                PORTA ^= (1 << PA3); //turns on led
                colbut = 1;
        }
        else{
                colbut = 0;
        }
}
     
