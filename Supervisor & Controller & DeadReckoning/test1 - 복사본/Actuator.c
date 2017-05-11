#include "Actuator.h"
#include <avr/io.h>

Actuator* make_actuator(){
	Actuator* actu = (Actuator*) malloc(sizeof(Actuator));
	actu->velocity=0;
	actu->angle=0;
	return actu;
}

void set_vout(Actuator* actu, float velo){
	actu->velocity = velo;
}

void set_aout(Actuator* actu, float ang){
	actu->angle = ang;
}

void activate_vout(Actuator* actu){
	/*
		perform an action for given velocity
	*/

}

void activate_aout(Actuator* actu){
	/* 
	perform an action for given angle
	*/
}

void velocity_act(float vel){
	if (vel==0)
	{	
		DDRB =0X50;
		PORTB |=0<<PB7; // P(B_7) -> MOTOR DRIVING LINE
	}
	else
	{	
		
			float ttl2=(vel*11.11)+500;
			ttl2=(unsigned int) ttl2;

		//	DDRB.7=1;
			PORTB |=1<<PB7;

			us_delay(ttl2);

			PORTB|=0<<PB7;
			us_delay(20000-ttl2);
		
	}
}

void steer_act(float steer){
	if (steer==0) 
	{	
		// DDRB.5=1;
		PORTB |= 0<<PB5; // P(B_5) -> steer control LINE
	}
	else
	{	
		float ttl=(steer*11.11)+500;
			ttl=(unsigned int) ttl;

		//	DDRB.5=1;
			PORTB |= 1<<PB5;

			us_delay(ttl);

			PORTB|=0<<PB5;
			us_delay(20000-ttl);
		
	}
}

/*
void stop(){
	check=0;
	velocity_act(0);
	steer_act(0);
}
*/
