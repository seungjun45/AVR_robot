#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include "Field.h"
#include "Actuator.h"
#include "node.h"
#include "Vehicle.h"
#include <math.h>

#define SONIC_INPUT_1 PORTD.0
#define SONIC_INPUT_2 PORTD.1
#define SONIC_INPUT_3 PORTD.2
#define SONIC_INPUT_4 PORTD.3


int toggle=0;


uint16_t timer0;
uint16_t remainder;

float ti=0,tf=0,del_t=0,old_del_t=0,vel=0,old_vel=0,old_a=0;
float del_theta=0, old_del_theta=0;
float del_x; 
float del_y; 
float a=0,d=34.2; // 34.2cm
float a1,a2,a3,a4,a5; // sonar sensor's angle!
float d1,d2,d3,d4,d5; // return value from sonar sensors
float v_desired, a_desired;
pos_node* direction;

float* v_control[2]; // input to PID CONTROLLER
float* sys_control_v[2]; // output of PID CONTROLLER
float* a_control[2];
float* sys_control_a[2];

DDRB.5 = 1;
DDRB.7 = 1;


Field* MYMAP;
Vehicle* MYROBOT;
Actuator* ACTUATOR;

ISR(TIMER0_OVF_vect){	// 
	
	timer0 ++;	// 
												
}

ISR(INT0_vect){
	
	if (toggle == 0){ // whenever interrupt first(!) occurs, set logic true.
		  timer0=0;
		  TIMSK0 |= 1<<TOIE0;	//  activate timer-interrupt 0
		  TCCR0A |= 1<<CS01;	// 8ck prescaler
		  TCNT0=0; // 
		 EICRA=0X02; // EXT_int0 falling edge interrupt
		 toggle=1;
	}
	else{
		remainder=(uint16_t) TCNT0;
		TIMSK0 = 0X00; // ALL timer disable
		toggle=0;
		EICRA=0X03; // INT0 high_edge interrupt
		timer0;
		d1=(timer0*128 + remainder*0.5)/59-10; // sonar1 distance update
		if (d1 < 50){ // obstacle detected
			pos_node* new_ob=new_obstacle(a1, d1,MYMAP);
			update_obstacle(new_ob,MYMAP);
		}

	}
	/*
	sonar_1
	if (d1 < threshold){ // obstacle detected
		pos_node* new_ob=new_obstacle(a1, d1,MYMAP);
		update_obstacle(new_ob,MYMAP);

	*/
}

ISR(INT1_vect){

	if (toggle == 0){ // whenever interrupt first(!) occurs, set logic true.
		  timer0=0;
		  TIMSK0 |= 1<<TOIE0;	//  activate timer-interrupt 0
		  TCCR0A |= 1<<CS01;	// 8ck prescaler
		  TCNT0=0; // 
		 EICRA=0X08; // EXT_int1 falling edge interrupt
		 toggle=1;
	}
	else{
		remainder=(uint16_t) TCNT0;
		TIMSK0 = 0X00; // ALL timer disable
		toggle=0;
		EICRA=0X0C; // INT1 high_edge interrupt
		timer0;
		d2=(timer0*128 + remainder*0.5)/59-10; // sonar2 distance update
		if (d2 < 50){ // obstacle detected
			pos_node* new_ob=new_obstacle(a2, d2,MYMAP);
			update_obstacle(new_ob,MYMAP);
		}
	}

	/*
	sonar_2
	if (d2 < threshold){ // obstacle detected
		pos_node* new_ob=new_obstacle(a2, d2,MYMAP);
		update_obstacle(new_ob,MYMAP);
	*/
}

ISR(INT2_vect){

	if (toggle == 0){ // whenever interrupt first(!) occurs, set logic true.
		  timer0=0;
		  TIMSK0 |= 1<<TOIE0;	//  activate timer-interrupt 0
		  TCCR0A |= 1<<CS01;	// 8ck prescaler
		  TCNT0=0; // 
		 EICRA=0X20; // EXT_int2 falling edge interrupt
		 toggle=1;
	}
	else{
		remainder=(uint16_t) TCNT0;
		TIMSK0 = 0X00; // ALL timer disable
		toggle=0;
		EICRA=0X30; // INT2 high_edge interrupt
		timer0;
		d3=(timer0*128 + remainder*0.5)/59-10; // sonar3 distance update

		if (d3 < 50){ // obstacle detected
			pos_node* new_ob=new_obstacle(a3, d3,MYMAP);
			update_obstacle(new_ob,MYMAP);
		}
	}
	/*
	sonar_3

	*/
}

ISR(INT3_vect){

	/*
	sonar_4
	if (d4 < threshold){ // obstacle detected
		pos_node* new_ob=new_obstacle(a4, d4,MYMAP);
		update_obstacle(new_ob,MYMAP);
	*/
}

ISR(INT4_vect){

	/*
	sonar_5
	if (d5 < threshold){ // obstacle detected
		pos_node* new_ob=new_obstacle(a5, d5,MYMAP);
		update_obstacle(new_ob,MYMAP);
	*/
}

ISR(INT5_vect){
	/* FIRST PRIORITY
	BACK_IR
	PORT?=0X00; (STOP RUNNING MOTOR)
	stop_vehicle(MYROBOT);
	*/
}

ISR(INT6_vect){

	/*
	veloctiy_encoder
	tf=Time(pulse2); // need code! after running TIMER
	old_del_t=del_t;
	del_t=tf-ti;
	old_vel=vel;
	old_a=a;
	vel=1.924/del_t;
	a=get_steer() // need code! get_steer() = getting analog input(with proper conversion) from potentiometer's output
	old_del_theta=del_theta;
	del_theta=del_t*(tan(a)*vel/d-tan(old_a)*old_vel/d);
	del_x=del_t*0.5*((d*(del_theta/del_t)/sin(a)*cos(a+get_dir(MYMAP)+del_theta)+vel*cos(get_dir(MYMAP)+del_theta))-(d*(old_del_theta/old_del_t)/sin(old_a)*cos(old_a+get_dir(MYMAP))+old_vel*cos(get_dir(MYMAP)));
	del_y=del_t*0.5*((d*(del_theta/del_t)/sin(a)*sin(a+get_dir(MYMAP)+del_theta)+vel*sin(get_dir(MYMAP)+del_theta))-(d*(old_del_theta/old_del_t)/sin(old_a)*sin(old_a+get_dir(MYMAP))+old_vel*sin(get_dir(MYMAP)));
	position_update(get_x(MYMAP)+del_x, get_y(MYMAP)+del_y, get_dir(MYMAP)+del_theta,MYMAP)
	<< change recommended -> change into functio form >>
	*/
}

int main()
{
	// EICRA |=0X??;  // interrupt condition setting 
	// EICRB |=0x??;  // interrupt condition setting 
	EIMSK |=(1<<INT6)|(1<<INT5)|(1<<INT4)|(1<<INT3)|(1<<INT2)|(1<<INT1)|(1<<INT0);
	
	/*

	DDR? = 0X??; 		// PIN
	PORT? = 0X??;		// setting

	*/
	initPort();
	// LCD_init(); // may be not required yet.
	MYMAP=make_field(0,0);
	MYROBOT=make_vehicle(0,0);
	ACTUATOR=make_actuator();
	v_control[0]=(float*) malloc(sizeof(float));
	v_control[1]=(float*) malloc(sizeof(float));
	sys_control_v[0]=(float*) malloc(sizeof(float));
	sys_control_v[1]=(float*) malloc(sizeof(float));
	a_control[0]=(float*) malloc(sizeof(float));
	a_control[1]=(float*) malloc(sizeof(float));
	sys_control_a[0]=(float*) malloc(sizeof(float));
	sys_control_a[1]=(float*) malloc(sizeof(float));
	*v_control[0]=0;
	*v_control[1]=0;
	*sys_control_v[0]=0;
	*sys_control_v[1]=0;
	*a_control[0]=0;
	*a_control[1]=0;
		
	sei();
	while(1){
	
	//sonar1 activation->deactivation
	EIMSK |=(1<<INT0);
	pulse(1);

	//sonar2 activation->deactivation
	EIMSK |=(1<<INT1);
	pulse(2);

	//sonar3 activation->deactivation
	EIMSK |=(1<<INT2);
	pulse(2);

	//sonar4 activation->deactivation
	//sonar5 activation->deactivation

	EIMSK |=(1<<INT6);

	//send data in MYMAP to laptop
	direction=get_obstacles(MYMAP);
	/* BELOW CODE FOR LAPTOP!
	while(direction != NULL){
		x=show_x(direction);
		y=show_y(direction);
		plot(x,y);
		hold on;
		direction=Jump_to_next(direction);
	}
	*/

	

	update_steer(MYROBOT,a); // set new values for "Vehicles' elements..(to avoid data sharing..)
	update_vel(MYROBOT,vel,get_x(MYMAP),get_y(MYMAP)); // ..


	// v_desired=FSM_velocity(???); // FSM's velocity must give desired velocity
	// a_desired=FSM_angle(???);
	// set_velocity(MYROBOT,v_desired) 
	// set_angle(MYROBOT,a_desired)

	//PID CONTROL PART
	*v_control[1]=MYROBOT->vel_desired-MYROBOT->vel_sensed;
	*a_control[1]=MYROBOT->st_ang_desired-MYROBOT->st_ang_sensed;
	//*sys_control_v[1]=k1*(*sys_control_v[0])+k2*(*v_control[0])+k3*(*v_control[1]);
	//*sys_control_a[1]=k4*(*sys_control_a[0])+k2*(*a_control[0])+k3*(*a_control[1]);
	*sys_control_v[0]=*sys_control_v[1];
	*v_control[0]=*v_control[1];

	*sys_control_a[0]=*sys_control_a[1];
	*a_control[0]=*a_control[1];



	// set_vout(ACTUATOR,*sys_control_v[1]);
	// set_aout(ACTUATOR,*sys_control_a[1]);
	activate_vout(ACTUATOR);
	activate_aout(ACTUATOR);
	}

	return 0;
}
