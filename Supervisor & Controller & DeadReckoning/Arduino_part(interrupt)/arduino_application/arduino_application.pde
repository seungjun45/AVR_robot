
uint16_t remainder;
volatile uint64_t times=0;
uint64_t tf;
int toggle=0;
int vel=0;


uint64_t i = 0;

double x=0;
double y=0;
double dir=0;
double the_deriv;
double d=2525;
double alpha;
double dummy1;
double dummy2;
double dummy3;
double dummy4;
double dummy5;
double del_x;
double del_y;

#define M_PI       3.14159265358979323846

// This is the INT0 Pin of the ATMega8
int steer = 0;
// We need to declare the data exchange
// variable to be volatile - the value is
// read from memory.
volatile int value = 0;


ISR(TIMER2_OVF_vect){	// 1 milisecond 
	TCNT2=6;
	times++;									
}

// Install the interrupt routine.
ISR(INT0_vect){
	if(toggle==0){
	
		if(times > 50){
			EIMSK |= 0X00;
			remainder=(uint16_t) TCNT2;	
			TCNT2=6;
			tf=times+i; // TCNT0 increment with 4us
			// tf = milisecond unit
			times=0;
			vel=98175/tf; // vel->unit= 0.1mm/ second
                        MCUCR |= ( 0 << ISC00);
                        MCUCR |= ( 1 << ISC01);

			toggle=1;
			i=0;
			
                        steer=analogRead(0);
			alpha=(steer*1000-689380)*M_PI/(7925*180);
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

			


			EIMSK |=(1<<INT0);
		}
		else{
			i=times+i;
			times=0;
		}
	}
	else{

		if(times > 50){	
			EIMSK |= 0X00;	
			remainder=(uint16_t) TCNT2;	
			TCNT2=6;
			tf=times+i; // TCNT0 increment with 4us
			// tf = milisecond unit
			times=0;
			vel=78540/tf; // vel-> unit= 0.1cm/ second
                        MCUCR |= ( 1 << ISC00);
                        MCUCR |= ( 1 << ISC01);
			toggle=0;
			i=0;

			steer=analogRead(0);
			alpha=(steer*1000-689380)*M_PI/(7925*180);
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


			EIMSK |=(1<<INT0);
			}
		else{
			i=times+i;
			times=0;
			
		}
	}
}


void setup() {
  noInterrupts();
  Serial.begin(9600);
  //Serial.println("Initializing ihandler");

 // Serial.println("Processing initialization");
  // Global Enable INT0 interrupt
  EIMSK |= ( 1 << INT0);

  TCNT0=0;
  MCUCR |= ( 1 << ISC00);
  MCUCR |= ( 0 << ISC01);
  TCCR2A |= 1<<CS21 | 1<<CS20;	// 256ck prescaler
  TIMSK2 |= 1<<TOIE2;	//  activate timer-interrupt 2
  interrupts();
  TCNT0=6;
 // Serial.println("Finished initialization");
}



void loop() {
  Serial.println(x);

  Serial.println(y);

  Serial.println(dir);
}


