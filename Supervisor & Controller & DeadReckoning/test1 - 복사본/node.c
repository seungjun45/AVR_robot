#include <stdio.h>
#include "node.h"

pos_node* make_node(float x_val,float y_val){
	pos_node* new_node = (pos_node*) malloc(sizeof(pos_node));
	new_node -> pos_x = x_val;
	new_node -> pos_y = y_val;
	new_node -> next = NULL;
	return new_node;
}

pos_node* con_node(float x_val,float y_val, pos_node* cdr_node){ // idea from (con A B) = (A (B NULL))
	pos_node* new_node = make_node(x_val,y_val);
	new_node -> next = cdr_node;
	return new_node;
} // address(pointer) of new node s.t (new_node cdr_node) is returned.

void prevent_overlap(float x_val,float y_val, pos_node* first_node){ // delete every node for corresponding x_pos and y_pos
	if (first_node == NULL){
		return;
	}
	else if ((first_node -> pos_x == x_val) && (first_node -> pos_y == y_val)){
		pos_node* new_head = first_node -> next;
		free(first_node);
		first_node=new_head;
		return prevent_overlap(x_val,y_val,first_node);
	}
	else{ // case the value is not find in this iteration.
		return prevent_overlap(x_val,y_val,first_node->next);
	}
}

float show_x(pos_node* obstacle){
	return obstacle->pos_x;
}

float show_y(pos_node* obstacle){
	return obstacle->pos_y;
}

pos_node* Jump_to_next(pos_node* obstacle){
	obstacle = obstacle->next;
	return obstacle;
}

	
