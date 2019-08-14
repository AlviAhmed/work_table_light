//GETTING BUTTON PRESSES AND INTERRUPTS TO WORK can't use since programming for actual leds instead of states
#define F_CPU 1000000UL 
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define redpress (~PINB) & (_BV(0)) //PB0 
#define partpress (~PINB) & (_BV(1)) //PB1  
#define redread PINA & (_BV(1)) //PA1 
#define partread PINA & (_BV(0))//PA0
int byte_val = 0;
int redbut = 0; //last button press for red button (PB0)    
int redenable = 0;
int partbut = 0;//last button press for part button (PB1)
int partenable = 0; 
int potenable = 1;
int main (void) {   
	init(); 
	pinint();
	while (1){       


	}  

	return (0);

} 

void init() { 
DDRB &= ~ (1 << PB0) | (1 << PB1);   //enabling input on button 
PORTB |= (1 << PB0) | (1 << PB1); //tunring on internal resistor
DDRA |= (1 << PA1) | (1 << PA0) | (1 << PA2); //enabling output of led 
PORTA &=~ 0x07 ;//initially off 
/*Party button = PA0 
  Red button = PA1 
  Default state button = PA2 */
} 


void pinint() { 
	GIMSK |= (1 << PCIE1); //enabling pin change interrupt
	PCMSK1 |= (1 << PB0) | (1 << PB1); //specifying the pins that cause interrupt 
	sei();
}  

void default_state() {

}

ISR (PCINT1_vect){ //PCINT1 takes care of pins PCINT11:8 
	_delay_ms(5);   
	if (partpress &&  partbut == 0 ){           
		if (PORTA == 0x02){
			PORTA = (PORTA & ~0x02) | 0x01;  
		}  
		else {  
			PORTA ^= 0x01; 
		}
		partbut = 1;  
	}   
	else {  
		partbut = 0;	  
	}
	if (redpress && redbut == 0){       
		if (PORTA == 0x01){ 
			PORTA = (PORTA & ~0x01) | 0x02;
		}  
		else {  
			PORTA ^= 0x02; 
		}
		redbut = 1; 
	}  
	else{   
		redbut = 0; 
	}            


	/*
	if (partpress && redpress && partbut == 0 && redbut == 0){//if both buttons pressed, default to one of them  
		PORTA ^= 0x04;
	
		redenable = 0;
		partenable = !partenable;  

		redbut = 1; 
	}else { 
		redbut = 0;
	}  
	*/

}
