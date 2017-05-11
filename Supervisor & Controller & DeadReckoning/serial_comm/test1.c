#define	F_CPU	16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdio.h>
#include "myCANLib.h"

// SONAR part
uint16_t timer0;
uint16_t remainder;
int toggle=0;
int distance1;
int distance2;
int distance3;

// PLANNING part
int myAngle;


// CAN part
struct MOb msg1={0x023, 0, EXT, 8, {0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38}}; 	
struct MOb msg2={0x027, 0, EXT, 8, {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00}};
struct MOb msg3;
struct MOb msg4;


// ENCODER part
uint16_t remainder2;
volatile uint64_t times=0;
uint64_t tf;
int toggle2;
uint64_t vel;
uint64_t i=0;

// ENCODER TIMER & INTERRUPT

ISR(TIMER2_OVF_vect){	// 1 milisecond 
	TCNT2=6;
	times++;	
												
}

ISR(INT6_vect){
	if(toggle2==0){
	
		if(times > 10){
			EIMSK = 0X00;
			remainder2=(uint16_t) TCNT2;	
			TCNT2=6;
			tf=times+i; // TCNT0 increment with 4us
			// tf = milisecond unit
			times=0;
			vel=98175/tf; // vel->unit= 0.1 milimeter/ second

			EICRB=0X20;
			toggle2=1;
			i=0;
			EIMSK |=(1<<INT6);
		}
		else{
			i=times+i;
			times=0;
		}
	}
	else{

		if(times > 10){	
			EIMSK = 0X00;	
			remainder2=(uint16_t) TCNT2;	
			TCNT2=6;
			tf=times+i; // TCNT0 increment with 4us
			// tf = milisecond unit
			times=0;
			vel=78540/tf; // vel-> unit= 0.1 milimeter/ second

			EICRB=0X30;
			toggle2=0;
			i=0;
			EIMSK |=(1<<INT6);
			}
		else{
			i=times+i;
			times=0;
			
		}
	}
}
// ENCODER TIMER & INTERRUPT END

// SONART TIMER & INTERRUPT

ISR(TIMER0_OVF_vect){	// 
	
	timer0 ++;	// 
												
}

ISR(INT0_vect){ // if toggle=1 -> amp is high.. toggle=0 -> amp is low	
	if (toggle == 0){ // whenever interrupt first(!) occurs, set logic true.
		  timer0=0;
		  TIMSK0 |= 1<<TOIE0;	//  activate timer-interrupt 0
		  TCCR0A |= 1<<CS01;	// 8ck prescaler
		  TCNT0=0; // 
		 EICRA=0X02; // EXT_int4 falling edge interrupt
		 toggle=1;
	}
	else{
		remainder=(uint16_t) TCNT0;
		TIMSK0 = 0X00; // ALL timer disable
		toggle=0;
		EICRA=0X03; // INT4 high_edge interrupt
		timer0; 
		
	}
	distance1=(int) (timer0*128 + remainder*0.5)/59;

	
}



ISR(INT1_vect){ // if toggle=1 -> amp is high.. toggle=0 -> amp is low
	if (toggle == 0){ // whenever interrupt first(!) occurs, set logic true.
		  timer0=0;
		  TIMSK0 |= 1<<TOIE0;	//  activate timer-interrupt 0
		  TCCR0A |= 1<<CS01;	// 8ck prescaler
		  TCNT0=0; // 
		 EICRA=0X08; // EXT_int4 falling edge interrupt
		 toggle=1;
	}
	else{
		remainder=(uint16_t) TCNT0;
		TIMSK0 = 0X00; // ALL timer disable
		toggle=0;
		EICRA=0X0C; // INT4 high_edge interrupt
		timer0; 
	
	}
	distance2=(int) (timer0*128 + remainder*0.5)/59;

	
}

ISR(INT3_vect){ // if toggle=1 -> amp is high.. toggle=0 -> amp is low

	if (toggle == 0){ // whenever interrupt first(!) occurs, set logic true.
		  timer0=0;
		  TIMSK0 |= 1<<TOIE0;	//  activate timer-interrupt 0
		  TCCR0A |= 1<<CS01;	// 8ck prescaler
		  TCNT0=0; // 
		 EICRA=0X80; // EXT_int6 falling edge interrupt
		 toggle=1;
	}
	else{
		remainder=(uint16_t) TCNT0;
		TIMSK0 = 0X00; // ALL timer disable
		toggle=0;
		EICRA=0XC0; // INT5 high_edge interrupt
		timer0; 

	}
	distance3=(int) (timer0*128 + remainder*0.5)/59;

	
}

// SONAR TIMER & INTERRUPT END

int main(void){
	/*
	can_init(b250k);		// CAN 초기화	

	can_rx_set(1, 0x25, EXT, 8, 0x1fffffff, 0x05);	// ID : 25
	can_rx_set(2, 0x28, EXT, 8, 0x1fffffff, 0x05);  // ID : 28
	*/
	ms_delay(300);
	sei();
	main_motor();
	//enable_encoder();
	toggle2=0;	


	while(1)
	{	
		
		sonar1_pulse();
		sonar2_pulse();
		sonar3_pulse();

		// tx
		/*
		msg1.data[0]=(char) (distance1 >> 4);	// 데이터 ASCII 부호화
		msg1.data[1]=(char) distance1 & (0x0 << 4) ;
		msg1.data[2]=(char) (distance2 >> 4);
		msg1.data[3]=(char) distance2 & (0x0 << 4) ;
		msg1.data[4]=(char) (distance3 >> 4);
		msg1.data[5]=(char) distance3 & (0x0 << 4) ;
		
		can_tx(7, &msg1,0);

		stop_pwm();

		// rx
		can_rx(1,&msg3);
		
		//  motion based on plan start
		myAngle=(int) msg3.data[0];
		*/
		myAngle=judge_dist(distance1,distance2,distance3);

		//if (myAngle > 145){
		//	stop_pwm();}
	//	else{
			steer_myAngle(myAngle);
			forward_pwm();;
	//	}
		// motion based on plan end
		/*

		//  tx
		msg3.data[0]=(char) (vel >> 4);
		msg3.data[1]=(char) vel & (0x0 << 4);

		can_tx(7, &msg2,0);

		// rx

		can_rx(2, &msg4);
		
		if(msg4.data[0]=0x00){
			stop_pwm();
			for (int k=0;k<3;k++){
				steer_myAngle(0);
				steer_myAngle(105);
				steer_myAngle(145);
				ms_delay(500);}
		} // checking if well_get.
	

		*/

	



		

		/*
		myAngle = judge_dist(distance1,distance2,distance3);
		
		
		if (myAngle > 145){
			stop_pwm();}
		else{
			steer_myAngle(myAngle);
			forward_pwm();;
		}
	
		*/

			

	}
	return 0;
	
}

