uint16_t min_dist(uint16_t distance1,uint16_t distance2,uint16_t distance3){
		uint16_t speed;
		if(distance1 < distance2){
			if(distance1 < distance3){
				speed=distance1;}
			else{
				speed=distance3;}
			}
		else{ // distance1 > distance2
			if(distance2 < distance3){
				speed=distance2;}
			else{
				speed=distance3;}
			}
}
