//GETTING BUTTON PRESSES AND INTERRUPTS TO WORK
#define F_CPU 8000000UL 
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define colpress (~PINB) & (_BV(0)) //PB0 

int colbut = 0; //last button press for red button (PB0)  

int main (void) {   
	init(); 
	pinint();
	while (1){  

	}

} 

void init() { 
DDRB &= ~ (1 << PB0);   //enabling input on button 
PORTB |= (1 << PB0); //tunring on internal resistor
DDRA |= (1 << PA3); //enabling output of led 
PORTA |=  (1 << PA3);//initially on
}

void pinint() { 
	GIMSK |= (1 << PCIE1); //enabling pin change interrupt
	PCMSK1 |= (1 << PB0); //specifying the pins that cause interrupt 
	sei();
} 

ISR (PCINT1_vect){ //PCINT1 takes care of pins PCINT11:8 
	_delay_ms(5);  
	if (colpress){//turning on first led 
		if (colbut == 0){ 
			PORTA ^= (1 << PA3); //turns on led
			colbut = 1; 
		} 
		
	}  
	else{  
		colbut = 0; 
	} 
	
}
