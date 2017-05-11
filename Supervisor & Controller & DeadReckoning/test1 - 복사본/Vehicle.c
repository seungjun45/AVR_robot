#include "Field.h"
#include "Vehicle.h"

Vehicle* make_vehicle(float init_a, float init_vel){
	Vehicle* a_vehicle = (Vehicle*) malloc(sizeof(Vehicle));
	a_vehicle->st_ang_sensed = init_a;
	a_vehicle->vel_sensed=init_vel;
	a_vehicle->st_ang_desired = 0;
	a_vehicle->vel_desired = 0;
	a_vehicle->position_x= 0;
	a_vehicle->position_y= 0;
	a_vehicle->Back_IR = 0; // IR detects sth...(0->no, 1->detect)
	a_vehicle->FW_OR_BW = 1; // Forward=1, Backward=0;
	return a_vehicle;
}

void update_steer(Vehicle* vehi, float a_steer){
	vehi->st_ang_sensed = a_steer;
}

void update_vel(Vehicle* vehi, float velocity,float new_x, float new_y){
	if((vehi->position_x ==new_x) && (vehi->position_y == new_y)){
		vehi->vel_sensed=0;
	}
	else vehi->vel_sensed=velocity;
}

void set_velocity(Vehicle* vehi,float v_desired){
	vehi->vel_desired=v_desired;
}

void set_angle(Vehicle* vehi, float a_desired){
	vehi->st_ang_desired=a_desired;
}

void set_ForB(Vehicle* vehi, int dir){
	vehi->FW_OR_BW=dir; // 1 for forward, 0 for backward
}

void stop_vehicle(Vehicle* vehi){
	vehi->Back_IR=1;
}

/*
void move_vehicle(Vehicle* vehi){
	if (pincode_IR SENSOR == LOW){
	 vehi->Back_IR = 0;
	}
}
*/
