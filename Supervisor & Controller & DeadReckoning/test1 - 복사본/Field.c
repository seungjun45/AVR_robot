
#include <stdio.h>
#include <math.h>
#include "node.h"
#include "Field.h"

/*
struct Field
{
  	float car_x;   // robot's position (x)
  	float car_y;   // robot's position (y)
	float car_dir; // robot's direction
  	pos_node* Obstacles; // linked list of obstacles' positions
};
*/

Field* make_field(float init_x,float init_y){ // constructor
	Field* new_field=(Field*) malloc(sizeof(Field));
	new_field->car_x = init_x;
	new_field->car_y = init_y;
	new_field->car_dir = 0;
	new_field->Obstacles = NULL;
	return new_field;
}

void position_update(float x, float y, float dir,Field* Map){ // method
	Map->car_x = x;
	Map->car_y = y;
	Map->car_dir= dir;
}

float get_x(Field* Map){
	return Map->car_x;
}

float get_y(Field* Map){
	return Map->car_y;
}

float get_dir(Field* Map){
	return Map->car_dir;
}

pos_node* new_obstacle(float steer_angle, float dist_to_obst,Field* Map){
	pos_node* new_ob = (pos_node*) malloc(sizeof(pos_node));
	new_ob->pos_x=dist_to_obst*cos(steer_angle+Map->car_dir)+Map->car_x;
	new_ob->pos_y=dist_to_obst*sin(steer_angle+Map->car_dir)+Map->car_y;
	new_ob->next=NULL;
	return new_ob;
}

void update_obstacle(pos_node* new_ob,Field* Map){ // cons(lisp language) new_ob to Map->Obstacles
	float x_val=new_ob->pos_x;
	float y_val=new_ob->pos_y;
	prevent_overlap(x_val,y_val,Map->Obstacles);
	new_ob->next=Map->Obstacles;
	Map->Obstacles=new_ob;
}

pos_node* get_obstacles(Field* Map){
	return Map->Obstacles;
}
  





