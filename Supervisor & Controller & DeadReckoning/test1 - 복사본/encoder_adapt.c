

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>





void enable_encoder(){
	TCCR2A |= 1<<CS21 | 1<<CS20;	// 64ck prescaler
	TIMSK2 |= 1<<TOIE2;	//  activate timer-interrupt 0
	TCNT2=0; //
	EICRB=0X30; // EXT_int6 rising edge interrupt
	EIMSK |=(1<<INT6);

	DDRE &= (1<<6);
	PORTE &= (1<<6);

	sei();
	TCNT2 = 6;
	
}

