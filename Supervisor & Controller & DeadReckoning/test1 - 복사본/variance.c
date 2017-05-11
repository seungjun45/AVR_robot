#include <stdio.h>

int variance_judge(uint16_t d1,uint16_t d2,uint16_t d3){
	uint16_t mean;
	mean=(d1+d2+d3)/3;
	uint16_t variance;
	variance=(uint16_t) ((d1-mean)^2+(d2-mean)^2+(d3-mean)^2);
	if(variance < 9){
		return 1;}
	else{
		return 0;}
}
	
uint16_t mean_dist(uint16_t d1,uint16_t d2,uint16_t d3){
	uint16_t mean;
	mean=(d1+d2+d3)/3;
	return mean;
}
