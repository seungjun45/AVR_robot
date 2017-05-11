//define LCD Port Pin ===========================================
#define LCD_RS_1  (PORTG |= 0x01)
#define LCD_RS_0  (PORTG &= 0xFE)

#define LCD_RW_1  (PORTG |= 0x02)
#define LCD_RW_0  (PORTG &= 0xFD)

#define EN_1  	  (PORTG |= 0x04)
#define EN_0      (PORTG &= 0xFB)

//LCD Functions =================================================
void E_Pulse(void);
void LCD_init(void);
void LCD_cmd(unsigned char cmd);
void Write_Char(unsigned char buf);
void LCD_Disp(char x,char y);
void LCD_Write(char x, char y,char *str);
void LCD_Write_char(char x, char y, unsigned char ch);
