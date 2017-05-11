int port;
int pin;

void pulse(int port_num){
		if(port_num==1){
			port=SONIC_INPUT_1;
			pin=DDRD.0;
		}
		else if(port_num==2){
			port=SONIC_INPUT_2;
			pin=DDRD.1;
		}
		else if(port_num==3){
			port=SONIC_INPUT_3;
			pin=DDRD.2;
		}

		pin=1;
		port=0; // E4 LOW

		us_delay(2);				

		pin=1;
		port=1; // E4 HIGH
		
		us_delay(5);

		pin=1;
		port=0; // E4 LOW

		pin=0;
		port=0; // E4 = digital input

		sei();  // want to be not used due to velocity encoder
		ms_delay(20);
		cli();
}
