#ifndef VEHICLE_H
#define VEHICLE_H

struct robot
{
	float st_ang_sensed;
	float vel_sensed;
	float st_ang_desired;
	float vel_desired;
	float position_x;
	float position_y;
	int Back_IR; // IR detects sth...(0->no, 1->detect)
	int FW_OR_BW; // Forward=1, Backward=0;
};
typedef struct robot Vehicle;




#endif VEHICLE_H
