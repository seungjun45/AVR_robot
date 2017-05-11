#define	F_CPU	16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdio.h>

int main(void){
	ms_delay(300);
	sei();
	main_motor();
	toggle2=0;	

	while(1)
	{	
			forward_pwm();
	}
	return 0;
}
