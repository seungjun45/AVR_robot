
int judge_dist(int dis1,int dis2,int dis3){
	int thres=50;
	int a3=0;
	int a2=105;
	int a1=140;
	int stop = 250;// stop signal;

	if(dis2 < thres){
		if(dis2 < 40){
			return stop;}
		else if(dis1 < thres){
			return a3;}
		else if(dis3 < thres){
			return a1;}
		else if((dis1 < thres) && (dis3 < thres)){
			return stop;}
		else{
			return a1;}
			}
	
	else if(dis1 < thres){
		if(dis2 < thres){
			return a3;}
		else if(dis3 < thres){
			return a2;}
		else if((dis2 < thres) && (dis3 < thres)){
			return stop;}
		else{
			return a3;}
			}

	else if(dis3 <thres){
		if(dis2 < thres){
			return a1;}
		else if(dis1 < thres){
			return a2;}
		else if((dis1 < thres) && (dis2 < thres)){
			return stop;}
		else a1;}
	else{
		return a2;
	}
}
			 


