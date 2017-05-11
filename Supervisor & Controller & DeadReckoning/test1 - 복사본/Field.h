#ifndef FIELD_H
#define FIELD_H

#include "node.h"



struct Field_Map
{
  	float car_x;   // robot's position (x)
  	float car_y;   // robot's position (y)
	float car_dir; // robot's direction
  	pos_node* Obstacles; // linked list of obstacles' positions
};
typedef struct Field_Map Field;


#endif FIELD_H
