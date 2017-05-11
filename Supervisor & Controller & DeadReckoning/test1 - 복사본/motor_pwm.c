#include <avr/io.h>
#include <util/delay.h>

void go(){

	PORTE = 0x0c; // 0000 1100
	_delay_ms(10);

	PORTE = 0x08; // 0000 1000
	_delay_ms(10);
	

}

void stop_pwm(){

	OCR3A = 0x0000;
	_delay_ms(100);

}

void reverse_pwm(){

	PORTA=0X01; // 1A -> HIGH, 2A->LOW

	TCCR3A &= ~(1<<COM3A0);				// Clear OCnA/OCnB/OCnC on Compare Match when up-counting.
											// Set OCnA/OCnB/OCnC on Compare Match when downcounting.
	 	OCR3A = 0x0fff; // <=출력 속도


}


void forward_pwm(){

	PORTA=0X02; // 1A -> LOW, 2A->HIGH

	TCCR3A &= ~(1<<COM3A0);				// Clear OCnA/OCnB/OCnC on Compare Match when up-counting.
											// Set OCnA/OCnB/OCnC on Compare Match when downcounting.
	 	OCR3A = 0x0fff; // <=출력 속도


}





void main_motor(){

	
	DDRE |=  (1<<DDE3);
	PORTE |= (1<<PE3);

	DDRA |= 0X03;
	
	TCCR3A = (1<<WGM31) | (1<<WGM30);	// Phase Correct, 10bit, PWM
	TCCR3A |= (1<<COM3A1);				// Clear OCnA/OCnB/OCnC on Compare Match when up-counting.
										// Set OCnA/OCnB/OCnC on Compare Match when downcounting.
	TCCR3B = (1<<CS31);					// clkI/O/8 (From prescaler)

	TCNT3 = 0x0000;
	
}

