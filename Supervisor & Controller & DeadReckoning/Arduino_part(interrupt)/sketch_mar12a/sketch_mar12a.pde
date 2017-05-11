
uint16_t remainder;
volatile uint64_t times=0;
uint64_t tf;
int toggle=0;
uint64_t vel;

uint64_t i = 0;

// This is the INT0 Pin of the ATMega8
int sensePin = 2;
// We need to declare the data exchange
// variable to be volatile - the value is
// read from memory.
volatile int value = 0;


ISR(TIMER0_OVF_vect){	// 1 milisecond 
	TCNT0=6;
	times++;									
}

// Install the interrupt routine.
ISR(INT4_vect){
	if(toggle==0){
	
		if(times > 50){
			EIMSK |= 0X00;
			remainder=(uint16_t) TCNT0;	
			TCNT0=6;
			tf=times+i; // TCNT0 increment with 4us
			// tf = milisecond unit
			times=0;
			vel=98175/tf; // vel->unit= 0.1 milimeter/ second
			EICRB=0X02; // falling edge
			toggle=1;
			i=0;
/*			
			alpha=(ADdata*1000-689380)*M_PI/(7925*180);
			dummy1=tan(alpha);
			dummy2=dummy1/d;
			the_deriv=dummy2*vel;
			dummy1=sin(alpha);
			dummy2=cos(alpha+dir);
			dummy3=cos(dir);
			dummy4=sin(alpha+dir);
			dummy5=sin(dir);
			del_x=tf*(d/dummy1*the_deriv*dummy2+vel*dummy3)/2;
			del_y=tf*(d/dummy1+the_deriv*dummy4+vel*dummy5)/2;
			x=x+del_x;
			y=y+del_y;
			dir=dir+the_deriv*tf;

*/				


			EIMSK |=(1<<INT4);
		}
		else{
			i=times+i;
			times=0;
		}
	}
	else{

		if(times > 50){	
			EIMSK |= 0X00;	
			remainder=(uint16_t) TCNT0;	
			TCNT0=6;
			tf=times+i; // TCNT0 increment with 4us
			// tf = milisecond unit
			times=0;
			vel=78540/tf; // vel-> unit= 0.1 milimeter/ second

			EICRB=0X03; //rising edge
			toggle=0;
			i=0;
/*
			alpha=(ADdata*1000-689380)*M_PI/(7925*180);
			dummy1=tan(alpha);
			dummy2=dummy1/d;
			the_deriv=dummy2*vel;
			dummy1=sin(alpha);
			dummy2=cos(alpha+dir);
			dummy3=cos(dir);
			dummy4=sin(alpha+dir);
			dummy5=sin(dir);
			del_x=tf*(d/dummy1*the_deriv*dummy2+vel*dummy3)/2;
			del_y=tf*(d/dummy1+the_deriv*dummy4+vel*dummy5)/2;
			x=x+del_x;
			y=y+del_y;
			dir=dir+the_deriv*tf;
*/

			EIMSK |=(1<<INT4);
			}
		else{
			i=times+i;
			times=0;
			
		}
	}
}


void setup() {
  Serial.begin(9600);
  Serial.println("Initializing ihandler");
  // sets the digital pin as output
  pinMode(ledPin, OUTPUT);
  // read from the sense pin
  pinMode(sensePin, INPUT);
  Serial.println("Processing initialization");
  // Global Enable INT0 interrupt
  EIMSK |= ( 1 << INT0);
  // Signal change triggers interrupt
  MCUCR |= ( 1 << ISC00);
  MCUCR |= ( 0 << ISC01);
  Serial.println("Finished initialization");
}

void loop() {
  if (value) {
    Serial.println("Value high!");
    digitalWrite(ledPin, HIGH);
  } else {
    Serial.println("Value low!");
    digitalWrite(ledPin, LOW);
  }
  delay(100);
}


