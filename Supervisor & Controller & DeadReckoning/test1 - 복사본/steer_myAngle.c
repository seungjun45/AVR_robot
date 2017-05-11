#include <avr/io.h>
#include "myDelay.h"

void steer_myAngle(int myAngle){
	DDRB |= (1<<3);
	unsigned char HIGH = 0X10; // PB4 -> STEER
	unsigned char LOW = 0X00;
	float* pulseWidth;
	unsigned int* pulse_Width;
	pulseWidth = (float*) malloc(sizeof(float));
	pulse_Width = (unsigned int*) malloc(sizeof(unsigned int)); 

	for(int k=0; k<=15; k++){
	*pulseWidth=(myAngle*11.11)+500;
	*pulse_Width=(unsigned int) *pulseWidth;
	PORTB=HIGH;
	us_delay(*pulse_Width);
	PORTB=LOW;
	us_delay(20000-*pulse_Width);
	}
}
