#include <stdio.h>
#include <tchar.h>
#include "SerialClass.h"	// Library described above
#include <string>

#include <cv.h>
#include <highgui.h>

#include <math.h>
using namespace std;

IplImage* imgTracking=0;

int lastX = -1;
int lastY = -1;

int xdirecval=0;
int ydirecval=0;





void arrowObject(IplImage* imgThresh, int *xdirec, int *ydirec){
	CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
    cvMoments(imgThresh, moments, 1);
    double moment10 = cvGetSpatialMoment(moments, 1, 0);
    double moment01 = cvGetSpatialMoment(moments, 0, 1);
    double area = cvGetCentralMoment(moments, 0, 0);
	int centerposX=0;
	int centerposY=0;

	CvSeq* contour;  //hold the pointer to a contour
    CvSeq* result;     //hold sequence of points of a contour
    CvMemStorage *storage = cvCreateMemStorage(0); //storage area for all contours
 
    cvFindContours(imgThresh, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));

     // if the area<1000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
             
    

	


	while(contour) // there may exist more than one arrow
       {
        //obtain a sequence of points of the countour, pointed by the variable 'countour'
       result = cvApproxPoly(contour, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contour)*0.02, 0);
           
       //if there are 7 vertices  in the contour and the area of the arrow is more than 1000 pixels
       if(result->total==7 && fabs(cvContourArea(result, CV_WHOLE_SEQ))>1000 )
       {
		   if(area>1000){
				 // calculate the center position of the arrow
				 centerposX = moment10/area;
				 centerposY = moment01/area;
              //iterating through each point
              CvPoint *pt[7];
              for(int i=0;i<7;i++){
                   pt[i] = (CvPoint*)cvGetSeqElem(result, i);
              }
   
               int delx;
               int dely;
			   int distance[7];
			  for(int k=0;k<7;k++){
				  distance[k]=(pow(double(pt[k]->x-centerposX),2.0)+pow(double(pt[k]->y-centerposY),2.0));
			  }

			  
			  int locat=0;
			  int locat2=1;
			  int dummy;
			  for(int t=2;t<7;t++){
				  if ((distance[locat] > distance[t]) || (distance[locat2] > distance[t])){
					  
						  dummy=t;
						  if(distance[locat]>distance[locat2]){ //nearest is locat2
							  locat=dummy;
						  }
						  else{
							  locat2=dummy;
						  }
				  }

			  }
			  

			  int midx=(pt[locat]->x+pt[locat2]->x)/2;
			  int midy=(pt[locat]->y+pt[locat2]->y)/2;
			 
			  delx=midx-centerposX;
			  dely=midy-centerposY;
			  
			  (*xdirec)=delx;
			  (*ydirec)=-dely; // inversion (1사분면이 x,y값 양수값)

             
			  cvLine(imgTracking, cvPoint(centerposX, centerposY), cvPoint(midx, midy), cvScalar(0,0,255), 4);

			  /*
			  cvLine(imgTracking, cvPoint(pt[0]->x, pt[0]->y), cvPoint(pt[1]->x, pt[1]->y), cvScalar(0,0,255), 4);
			  cvLine(imgTracking, cvPoint(pt[1]->x, pt[1]->y), cvPoint(pt[2]->x, pt[2]->y), cvScalar(0,0,255), 4);
			  cvLine(imgTracking, cvPoint(pt[2]->x, pt[2]->y), cvPoint(pt[3]->x, pt[3]->y), cvScalar(0,0,255), 4);
			  cvLine(imgTracking, cvPoint(pt[3]->x, pt[3]->y), cvPoint(pt[4]->x, pt[4]->y), cvScalar(0,0,255), 4);
			  cvLine(imgTracking, cvPoint(pt[4]->x, pt[4]->y), cvPoint(pt[5]->x, pt[5]->y), cvScalar(0,0,255), 4);
			  cvLine(imgTracking, cvPoint(pt[5]->x, pt[5]->y), cvPoint(pt[6]->x, pt[6]->y), cvScalar(0,0,255), 4);
			  cvLine(imgTracking, cvPoint(pt[6]->x, pt[6]->y), cvPoint(pt[0]->x, pt[0]->y), cvScalar(0,0,255), 4);
			  */

			  if(lastX >= 0 && lastY >= 0){
				  cvLine(imgTracking, cvPoint(centerposX, centerposY), cvPoint(lastX, lastY), cvScalar(255,0,0), 4);
			  }
			  lastX=centerposX;
			  lastY=centerposY;
              }
	   }
               
            
  
            //obtain the next contour
            contour = contour->h_next; 
      }

     free(moments); 
	}


IplImage* GetThresholdedImage(IplImage* imgHSV){       
    IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
    cvInRangeS(imgHSV, cvScalar(0,160,60), cvScalar(30,256,256), imgThresh); 
    return imgThresh;
}

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

// application reads from the specified serial port and reports the collected data
int _tmain(int argc, _TCHAR* argv[])
{
	printf("Welcome to the serial test app!\n\n");

	Serial* SP = new Serial("\\\\.\\COM6");    // adjust as needed
	Serial* DP = new Serial("\\\\.\\COM5");

	if (SP->IsConnected())
		printf("We're connected");

	char dir1[3]="";			// don't forget to pre-allocate memory
	char dir2[3]="";
	char dir3[3]="";

	char datax[3]="";
	char datay[3]="";
	char dir[3]="";
	char backbump[3]="";

	int right = 0;
	int straight = 105;
	int left = 145;
	int back= 250;

	int ob1;
	int ob2;
	int ob3;
	int myAngle=0;

	int x;
	int y;
	int direc;
	int direc_old=0;

	int toggler=10;

	//printf("%s\n",incomingData);
	int dataLength = 2;
	int readResult = 0;
	
	char reverse='r'; // reverse command to Arduino
	char forward='f'; // forward command to Arduino

	char motor='f'; // initial setting

	char request='r';

	char request1='e';



	while(SP->IsConnected() && DP->IsConnected()) // AVR connection
	{	

		DP->WriteData(&motor,1); // 1번
		
		DP->ReadData(datax,2);  // 2번
		DP->ReadData(datay,2);
		DP->ReadData(dir,2);
		//DP->ReadData(backbump,1);
		Sleep(200);


		
		SP->WriteData(&request1,1); // AVR obstacle position info request            3번
		
		SP->ReadData(dir1,dataLength); // obstacle received                4번
		SP->ReadData(dir2,dataLength); // obstacle received
		SP->ReadData(dir3,dataLength); // obstacle received
		

		//std::string test(incomingData);

		x=((int) datax[0])*127+((int) datax[1]); //                     5번
		y=((int) datay[0])*127+((int) datay[1]);
		direc=((int) dir[0])*127+((int) dir[1]);

		ob1=((int) dir1[0])*127+((int) dir1[1]); //                     5번
		ob2=((int) dir2[0])*127+((int) dir2[1]);
		ob3=((int) dir3[0])*127+((int) dir3[1]);
		
		printf("%i",x);
		printf("%i",y);
		printf("%i\n",direc);
		//printf("%i\n",ob1);
		//printf("%i\n",ob2);
		//printf("%i\n",ob3);
		
		// opencv!!

		CvCapture *capture =      cvCaptureFromCAM(0);

    if(!capture){
        printf("Capture failure\n");
        return -1;
    }
      
    IplImage* frame=0;
    frame = cvQueryFrame(capture);           
    if(!frame) return -1;
   
    //create a blank image and assigned to 'imgTracking' which has the same size of original video
    imgTracking=cvCreateImage(cvGetSize(frame),IPL_DEPTH_8U, 3);
    cvZero(imgTracking); //covert the image, 'imgTracking' to black

    cvNamedWindow("Video");
	while(toggler>0){
		toggler=toggler-1;
        frame = cvQueryFrame(capture);           
        if(!frame) break;
        frame=cvCloneImage(frame); 
         
        //smooth the original image using Gaussian kernel
        cvSmooth(frame, frame, CV_GAUSSIAN,3,3); 

        //converting the original image into grayscale
      IplImage* imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); 
      cvCvtColor(frame,imgHSV,CV_BGR2HSV);

	  IplImage* imgThresh = GetThresholdedImage(imgHSV);
	  cvSmooth(imgThresh, imgThresh, CV_GAUSSIAN,3,3);
          
       
            
        //track the possition of the ball
        arrowObject(imgThresh, &xdirecval, &ydirecval);

        // Add the tracking image and the frame
        cvAdd(frame, imgTracking, frame);
             
        cvShowImage("Video", frame);
		cvShowImage("Ball", imgThresh); 
		
   
        //Clean up used images
        cvReleaseImage(&imgThresh);            
        cvReleaseImage(&frame);
		cvReleaseImage(&imgHSV);

        Sleep(10);

		printf("%d",xdirecval);
		printf("%d\n",ydirecval);
	}
	toggler=10;
		if((!(xdirecval==0)) || (!(ydirecval==0))){
			direc_old=direc;
			if(xdirecval>0 || ydirecval >0){
				if(xdirecval>ydirecval){
					myAngle=judge_dist(ob1,ob2,ob3,right);//right priority
				}
				else{
					myAngle=judge_dist(ob1,ob2,ob3,straight);// straight forward priority
				}				
			}
			else if(xdirecval<0 || ydirecval <0){
				if (xdirecval < ydirecval){
					myAngle=judge_dist(ob1,ob2,ob3,left);//left priority
				}
				else{
					myAngle=judge_dist(ob1,ob2,ob3,back);// back priority
				}
			}
			else if(xdirecval>ydirecval){ // x plus y minus
				if(abs(xdirecval)>abs(ydirecval)){
					myAngle=judge_dist(ob1,ob2,ob3,right);//right priority
				}
				else{
					myAngle=judge_dist(ob1,ob2,ob3,back);//back priority
				}
			}
			else{ //x minus y plus
				if(abs(xdirecval)>abs(ydirecval)){
					myAngle=judge_dist(ob1,ob2,ob3,left);//left priority
				}
				else{
					myAngle=judge_dist(ob1,ob2,ob3,back);//back priority
				}
			}
		}
		else{
			myAngle=judge_dist(ob1,ob2,ob3,straight);
		}
		
		// opencv!!  and planning!! (backbump must be considered)
		xdirecval=0;
		ydirecval=0;
		Sleep(100);
		if(myAngle==250){
			motor=reverse;
			myAngle=105;
		}
		else{
			motor=forward;
		}
		SP->WriteData(&motor,1);   // forward or reverse (motor)
		char myAngle_= (char) myAngle; // after judgement (after planning)
		Sleep(2000);
		SP->WriteData(&myAngle_,1);  // angle?
		Sleep(2000);
		

		
	}
	return 0;
}