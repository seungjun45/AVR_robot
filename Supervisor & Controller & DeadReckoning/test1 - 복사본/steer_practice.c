#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

uint16_t myAngle=0;
float* pulseWidth;
unsigned char table[16] = {0};
unsigned int* pulse_Width;
unsigned char HIGH = 0X01; // CONTROLLING PIN PA0!
unsigned char LOW = 0X00;



void steer_practice(uint16_t myAngle){
	DDRA=0XFF;
	*pulseWidth=(myAngle*11.11)+500;
	*pulse_Width=(unsigned int) *pulseWidth;
	PORTA=HIGH;
	us_delay(*pulse_Width);
	PORTA=LOW;
	us_delay(20000-*pulse_Width);
}
