#include <avr/io.h>
#include "myDelay.h"

void sonar1_pulse(){ // PD0
	EICRA =0X03; // INT4의 #rising edge 이 인터럽트 요청을 발생함
	
	EIMSK |=(1<<INT0);	

	

	DDRD = 0XFF;
	PORTD = 0X00; // D0 LOW

	us_delay(2);		
			
	DDRD = 0XFF;
	PORTD = 0XFF; // D0 HIGH
	
	us_delay(5);

	DDRD = 0XFF;
	PORTD = 0X00; // D0 LOW

	DDRD=0X00;
	PORTD=0X00; // D0 = digital input


	ms_delay(20);


	EIMSK &=(0<<INT0);
}

void sonar2_pulse(){ // PD1
	EICRA =0X0C; // INT4의 #rising edge 이 인터럽트 요청을 발생함
	EIMSK |=(1<<INT1);	

	

	DDRD = 0XFF;
	PORTD = 0X00; // D1 LOW

	us_delay(2);		
			
	DDRD = 0XFF;
	PORTD = 0XFF; // D1 HIGH
	
	us_delay(5);

	DDRD = 0xFF;
	PORTD = 0X00; // D1 LOW

	DDRD=0X00;
	PORTD=0X00; // D1 = digital input


	ms_delay(20);


	EIMSK &=(0<<INT1);
}

void sonar3_pulse(){ // PE5
	EICRA =0X80; // INT2의 #rising edge 이 인터럽트 요청을 발생함
	EIMSK |=(1<<INT3);	

	

	DDRD = 0XFF;
	PORTD =0X00; // D2 LOW

	us_delay(2);		
			
	
	PORTD = 0XFF; // D2 HIGH
	
	us_delay(5);

	
	PORTD = 0X00; // D2 LOW

	DDRD = 0X00;
	PORTD = 0X00; // D2 = digital input


	ms_delay(20);

	EIMSK &=(0<<INT3);
}
