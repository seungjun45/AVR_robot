#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

uint16_t myAngle=0;
float* pulseWidth;
unsigned char table[16] = {0};
unsigned int* pulse_Width;

ISR(INT0_vect){ // increase angle by 1' with switch

	myAngle=myAngle+5;

	sprintf(table,"Angle%3d", myAngle);
	LCD_Write(0,0,table);

}

ISR(INT1_vect){
	
	myAngle=myAngle-5;

	sprintf(table,"Angle%3d",myAngle);
	LCD_Write(0,0,table);
}


int main()
{
	
	pulseWidth = (float*) malloc(sizeof(float));
	pulse_Width = (unsigned int*) malloc(sizeof(unsigned int)); 
	EICRA |=0X05;	 // any logical change
	EIMSK |=(1<< INT0)|(1<<INT1);

	DDRD=0X00;
	PORTD=0XFF; // PULL UP

	DDRA=0XFF;
	
	unsigned char HIGH = 0X01; // CONTROLLING PIN PA0!
	unsigned char LOW = 0X00;

	initPort();
	LCD_init();

	sei();
	// use disabling interrupt method.
	while(1){
	*pulseWidth=(myAngle*11.11)+500;
	*pulse_Width=(unsigned int) *pulseWidth;
	PORTA=HIGH;
	us_delay(*pulse_Width);
	PORTA=LOW;
	us_delay(20000-*pulse_Width);
	}
}

	
	
	
