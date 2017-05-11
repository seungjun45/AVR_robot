#include <avr/io.h>
#include "myDelay.h"

//define LCD Port Pin ===========================================
#define LCD_RS_1  (PORTG |= 0x01)
#define LCD_RS_0  (PORTG &= 0xFE)

#define LCD_RW_1  (PORTG |= 0x02)
#define LCD_RW_0  (PORTG &= 0xFD)

#define EN_1  	  (PORTG |= 0x04)
#define EN_0      (PORTG &= 0xFB)

//LCD Functions =================================================
void E_Pulse(void)
{
	EN_1;

	us_delay(100);

	EN_0;
}

void LCD_init(void)
{
	ms_delay(40);

	PORTC = 0x38;	// Function Set
	E_Pulse();
    us_delay(40);

	PORTC = 0x0c; // DisPlay ON/OFF Control
	us_delay(40);
	E_Pulse();
	
	PORTC = 0x01; // Display Clear
	ms_delay(2);
	E_Pulse();

	PORTC = 0x06; // Entry Mode Set
	E_Pulse();
}

void LCD_cmd(unsigned char cmd)
{
	LCD_RS_0;
	LCD_RW_0;
	PORTC=cmd;
	E_Pulse();
}	

void Write_Char(unsigned char buf)
{
	LCD_RS_1;
	LCD_RW_0;
	PORTC=buf;
	E_Pulse();
}	

void LCD_Disp(char x,char y)
{
	LCD_RS_0;
	LCD_RW_0;

	if(y==0) PORTC = x + 0x80;
	else if(y==1) PORTC = x + 0xc0;
	E_Pulse();
}
 
void LCD_Write(char x, char y,char *str)
{
	LCD_Disp(x,y);
	while(*str)
	Write_Char(*str++);
}

void LCD_Write_char(char x, char y, unsigned char ch)
{
	LCD_Disp(x,y);
	Write_Char(ch);
}
