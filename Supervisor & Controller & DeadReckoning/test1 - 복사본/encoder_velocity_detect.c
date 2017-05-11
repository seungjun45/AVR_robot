#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

uint16_t pulse_count=0;
uint32_t distance=0;
float velocity;
unsigned char velocity_d[16] = {0};
unsigned char pulse_count_val[16] = {0};
unsigned char time_count_t[16] = {0};
unsigned char distance_d[16] = {0};
int* time_count;
int* logic;

ISR(INT4_vect){

	if (*logic == 0){ // whenever interrupt first(!) occurs, set logic true.
		 *logic = 1;
		 *time_count=0;
	}
			
	pulse_count=pulse_count+1;
	
	
}

void time_counter()
{
	ms_delay(1);
	if (*time_count < 500){
		*time_count=*time_count+1;
	}
};


int main(){
	
//	velocity = (float*) malloc(sizeof(float));
	time_count = (int*) malloc(sizeof(int));
	logic = (int*) malloc(sizeof(int));
	
	EICRB |=0X02; // INT4의 #low level 이 인터럽트 요청을 발생함
	EIMSK |=(1<<INT4);

	DDRE = 0X00;
	PORTE = 0X00;
	
	initPort();
	LCD_init();

	pulse_count=0;
	
	sei(); // set enable interrupts

	while(1) {
	*logic=0;
	*time_count=500;
	for(int clck=0 ; clck<500 ; clck++){
		time_counter();
	}
	velocity=(pulse_count*3.848451001)*500/(*time_count);// actually, about 384.8451001
	distance=(int) *time_count*velocity/1000; // distance per 0.5 sec

	sprintf(velocity_d,"VEL%5d", (int)velocity);
//	sprintf(pulse_count_val,"Puls%4d",pulse_count);
	sprintf(distance_d,"dis%5d",distance);
	
//	LCD_Write(0,0,pulse_count_val);
	LCD_Write(0,1,distance_d);
	LCD_Write(0,0,velocity_d);
	pulse_count=0;

	

	

	}
}
