#define	F_CPU	16000000UL

#include <avr/io.h>
#include <stdio.h>

#include "myCANLib.h"
#include "myDelay.h"
#include "lcdControl.h"

//     MOb msg1={ID, RTR, IDE, DLC, DATA}
struct MOb msg1={0x023, 0, EXT, 8, {0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38}}; 	
struct MOb msg2={0x027, 0, EXT, 8, {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00}};
struct MOb msg3;
struct MOb msg4;



int main(void)
{
	char strBuff_1[20]={0};
	char strBuff_2[20]={0};
	char count;





	can_init(b250k);		// CAN 초기화	

	can_rx_set(1, 0x25, EXT, 8, 0x1fffffff, 0x05);	// ID : 25
	can_rx_set(1, 0x28, EXT, 8, 0x1fffffff, 0x05);  // ID : 28

	ms_delay(300);

	count=0;
	while(1){
		count++;
		if(count==10)
			count=0;

		msg1.data[4]=count+0x30;	// 데이터 ASCII 부호화
	
		
		can_tx(7, &msg1,0);	// CAN 메시지 송신
		can_rx(1, &msg2);	// CAN 메시지 송신

		sprintf(strBuff_1,"ID35/TxData=%c", msg1.data[4]);
		LCD_Write(0,0,strBuff_1);

		sprintf(strBuff_2,"RID=%ld D4:D5=%c:%c", msg2.id, msg2.data[4],msg2.data[5]);
		LCD_Write(0,1,strBuff_2);	

		ms_delay(500);

	
	}
	
	return 0;
}
