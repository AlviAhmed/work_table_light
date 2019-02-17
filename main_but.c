//GETTING BUTTON PRESSES AND INTERRUPTS TO WORK
#define F_CPU 8000000UL 
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#define redpress (~PINB) & (_BV(0)) //PB0 
#define partpress (~PINB) & (_BV(1)) //PB1 

int redbut = 0; //last button press for red button (PB0)    
int redenable = 0;
int partbut = 0;//last button press for part button (PB1)
int partenable = 0; 
int potenable = 1;
int main (void) {   
	init(); 
	pinint();
	while (1){    
		_delay_us(1);
		if (partenable == 1){ 
			PORTA |= (1 << PA0);   
		}else { 
			PORTA &=~ (1 << PA0);  
		}   
		if (redenable == 1){ 
			PORTA |= (1 << PA1);   
		}else { 
			PORTA &=~ (1 << PA1); 
		}   
	} 
	return (0);

} 

void init() { 
DDRB &= ~ (1 << PB0) | (1 << PB1);   //enabling input on button 
PORTB |= (1 << PB0) | (1 << PB1); //tunring on internal resistor
DDRA |= (1 << PA0) | (1 << PA1) | (1 << PA2); //enabling output of led 
PORTA &=~  (1 << PA0) | (1 << PA1) | (1 << PA2);//initially off
}

void pinint() { 
	GIMSK |= (1 << PCIE1); //enabling pin change interrupt
	PCMSK1 |= (1 << PB0) | (1 << PB1); //specifying the pins that cause interrupt 
	sei();
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
		redbut = 1; 
	}else { 
		redbut = 0;
	} 

}
