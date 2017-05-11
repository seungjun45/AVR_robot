#define	F_CPU	16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

uint16_t timer0;
uint16_t remainder;

int toggle=0;
volatile int check=1;
float distance;
unsigned char distance_d[16] = {0};
unsigned char flux[8] = {0};


ISR(TIMER0_OVF_vect){	// 
	
	timer0 ++;	// 
												
}

ISR(INT4_vect){ // if toggle=1 -> amp is high.. toggle=0 -> amp is low
	
	if (toggle == 0){ // whenever interrupt first(!) occurs, set logic true.
		  timer0=0;
		  TIMSK0 |= 1<<TOIE0;	//  activate timer-interrupt 0
		  TCCR0A |= 1<<CS01;	// 8ck prescaler
		  TCNT0=0; // 
		 EICRB=0X02; // EXT_int4 falling edge interrupt
		 toggle=1;
	}
	else{
		remainder=(uint16_t) TCNT0;
		TIMSK0 = 0X00; // ALL timer disable
		toggle=0;
		EICRB=0X03; // INT4 high_edge interrupt
		timer0; 
		check=0;
	}
	
	
}

int main(void){


	initPort();
	LCD_init(); // activating LCD

	
	

	// OCR0A = 1; // timer0 updated / 1 micro second.

	
	

	
	while(1)
	{
		EICRB |=0X03; // INT4의 #rising edge 이 인터럽트 요청을 발생함
		EIMSK |=(1<<INT4);
		DDRE=0XFF;
		PORTE=0X00; // E4 LOW

		us_delay(2);				

		DDRE=0XFF;
		PORTE=0XFF; // E4 HIGH
		
		us_delay(5);

		DDRE=0XFF;
		PORTE=0X00; // E4 LOW

		DDRE=0X00;
		PORTE=0X00; // E4 = digital input

		sei();
		ms_delay(20);
		cli();


		distance=(timer0*128 + remainder*0.5)/59;

		sprintf(distance_d,"dis%5d",(uint16_t) distance-10 ); // -10 -> unwanted error....(why?)
		LCD_Write(0,0,distance_d);
		ms_delay(100);

			

	}
	return 0;
	
}
