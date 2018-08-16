//GETTING BUTTON PRESSES AND INTERRUPTS TO WORK
#define F_CPU 8000000UL 
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define colpress (~PINB) & (_BV(0)) //PB0 
#define partpress (~PINB) & (_BV(1)) //PB1 

int colbut = 0; //last button press for red button (PB0)   
int partbut = 0;//last button press for part button (PB1)

int main (void) {   
	init(); 
	pinint();
	while (1){  

	}

} 

void init() { 
DDRB &= ~ (1 << PB0) | (1 << PB1);   //enabling input on button 
PORTB |= (1 << PB0) | (1 << PB1); //tunring on internal resistor
DDRA |= (1 << PA3) | (1 << PA2); //enabling output of led 
PORTA &=~  (1 << PA3) | (1 << PA2);//initially off
}

void pinint() { 
	GIMSK |= (1 << PCIE1); //enabling pin change interrupt
	PCMSK1 |= (1 << PB0) | (1 << PB1); //specifying the pins that cause interrupt 
	sei();
} 

ISR (PCINT1_vect){ //PCINT1 takes care of pins PCINT11:8 
	_delay_ms(10);   
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
