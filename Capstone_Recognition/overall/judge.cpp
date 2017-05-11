int judge_dist(int dis1,int dis2,int dis3, int priority){
	int thres=50;
	int a3=0;  //right
	int a2=105; //straight
	int a1=140;//left
	int back = 250;// stop signal;

	if(dis2 < thres){
		if(dis2 < 40){
			return back;}
		else if(dis1 < thres){
			return a3;}
		else if(dis3 < thres){
			return a1;}
		else if((dis1 < thres) && (dis3 < thres)){
			return back;}
		else{
			return priority;}
			}
	
	else if(dis1 < thres){
		if(dis2 < thres){
			return a3;}
		else if(dis3 < thres){
			return a2;}
		else if((dis2 < thres) && (dis3 < thres)){
			return back;}
		else if(dis1 < 40){
			return back;}
		else{
			return priority;}
			}

	else if(dis3 <thres){
		if(dis2 < thres){
			return a1;}
		else if(dis1 < thres){
			return a2;}
		else if((dis1 < thres) && (dis2 < thres)){
			return back;}
		else if(dis3 < 40){
			return back;}
		else priority;}
	else{
		return priority;
	}
}