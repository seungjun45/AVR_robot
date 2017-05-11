#define	F_CPU	16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

uint16_t remainder;
volatile uint64_t times=0;
uint16_t ttl;
uint64_t tf;
int toggle;
uint64_t vel;
uint64_t xet;
unsigned char velocity_d[16] = {0};
unsigned char time_d[16] = {0};
float pulse=4.000000000000989865465;
uint64_t i=0;

ISR(TIMER0_OVF_vect){	// 1 milisecond 
	TCNT0=6;
	times++;	
												
}

ISR(INT4_vect){
	if(toggle==0){
	
		if(times > 10){
			EIMSK |= 0X00;
			remainder=(uint16_t) TCNT0;	
			TCNT0=6;
			tf=times+i; // TCNT0 increment with 4us
			// tf = milisecond unit
			times=0;
			vel=98175/tf; // vel->unit= 0.1 milimeter/ second
			sprintf(velocity_d,"VEL%5u", vel); // use toggle for debugging rapid change due to noise in here!!!
			EICRB=0X02;
			toggle=1;
			i=0;
			EIMSK |=(1<<INT4);
		}
		else{
			i=times+i;
			times=0;
		}
	}
	else{

		if(times > 10){	
			EIMSK |= 0X00;	
			remainder=(uint16_t) TCNT0;	
			TCNT0=6;
			tf=times+i; // TCNT0 increment with 4us
			// tf = milisecond unit
			times=0;
			vel=78540/tf; // vel-> unit= 0.1 milimeter/ second
			sprintf(velocity_d,"VEL%5u", vel);// use toggle for debugging rapid change due to noise in here!!!

			EICRB=0X03;
			toggle=0;
			i=0;
			EIMSK |=(1<<INT4);
			}
		else{
			i=times+i;
			times=0;
			
		}
	}
}





	
	
		
	


int main()
{
	
	TCCR0A |= 1<<CS01 | 1<<CS00;	// 64ck prescaler
	TIMSK0 |= 1<<TOIE0;	//  activate timer-interrupt 0
	TCNT0=0; //
	EICRB=0X03; // EXT_int4 LOW LEVEL interrupt
	EIMSK |=(1<<INT4);

	DDRE = 0X00;
	PORTE = 0X00;
	
	initPort();
	LCD_init();
	sei();
	TCNT0 = 6;
	toggle=0;
	while(1){
	ttl=TCNT0;

	sprintf(time_d,"DIS%5u", times); // milisecond unit
	LCD_Write(0,0,velocity_d);
	LCD_Write(0,1,time_d);
	
	
	
	
	}
	return 0;
}
