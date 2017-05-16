#include <stdio.h>
#include <tchar.h>
#include "SerialClass.h"	// Library described above
#include <string>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <math.h>
using namespace std;

IplImage* imgTracking=0;

int lastX = -1;
int lastY = -1;

int xdirecval=0;
int ydirecval=0;

int xdirecv[10];
int ydirecv[10];





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
       if(result->total==7 && fabs(cvContourArea(result, CV_WHOLE_SEQ))>3000 )
       {
		   if(area>3000){
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


void xmarkObject(IplImage* imgThresh, char *myAngle){
	CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
	cvMoments(imgThresh, moments, 1);
	double moment10 = cvGetSpatialMoment(moments, 1, 0);
	double moment01 = cvGetSpatialMoment(moments, 0, 1);
	double area = cvGetCentralMoment(moments, 0, 0);
	int centerposX = 0;
	int centerposY = 0;

	CvSeq* contour;  //hold the pointer to a contour
	CvSeq* result;     //hold sequence of points of a contour
	CvMemStorage *storage = cvCreateMemStorage(0); //storage area for all contours

	cvFindContours(imgThresh, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

	// if the area<1000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 






	while (contour) // there may exist more than one arrow
	{
		//obtain a sequence of points of the countour, pointed by the variable 'countour'
		result = cvApproxPoly(contour, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contour)*0.02, 0);

		//if there are 7 vertices  in the contour and the area of the arrow is more than 1000 pixels
		if (result->total == 12 && fabs(cvContourArea(result, CV_WHOLE_SEQ))>3000)
		{
			if (area>3000){
				(*myAngle) = 's';
			}
		}



		//obtain the next contour
		contour = contour->h_next;
	}

	free(moments);
}





// application reads from the specified serial port and reports the collected data
int _tmain(int argc, _TCHAR* argv[])
{
	printf("Welcome to the serial test app!\n\n");


	Serial* SP = new Serial("\\\\.\\COM5");

	if (SP->IsConnected())
		printf("We're connected");


	int y_ratio;
	int x_ratio;
	

	char datax[3]="";
	char datay[3]="";
	char dir[3]="";
	char backbump[3]="";


	unsigned char back= 'b';

	int ob1;
	int ob2;
	int ob3;
	char myAngle;
	char myAngles[10];
	char direc;

	int x;
	int y;
	
	int direc_old=0;
	int cnt = 0;
	

	int toggler=10;

	//printf("%s\n",incomingData);
	int dataLength = 2;
	int readResult = 0;
	
	char reverse='r'; // reverse command to Arduino
	char forward='f'; // forward command to Arduino

	char motor='f'; // initial setting

	unsigned char request='r';

	for (int idx = 0; idx < 10; idx++){
		xdirecv[idx] = 0;
		ydirecv[idx] = 0;
	}




	while (SP->IsConnected()) // AVR connection
	{







		CvCapture *capture = cvCaptureFromCAM(1);

		if (!capture){
			printf("Capture failure\n");
			return -1;
		}

		IplImage* frame = 0;
		frame = cvQueryFrame(capture);
		if (!frame) return -1;

		//create a blank image and assigned to 'imgTracking' which has the same size of original video
		imgTracking = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3);
		cvZero(imgTracking); //covert the image, 'imgTracking' to black

		cvNamedWindow("Video");
		while (toggler < 10){

			frame = cvQueryFrame(capture);
			if (!frame) break;
			frame = cvCloneImage(frame);

			//smooth the original image using Gaussian kernel
			cvSmooth(frame, frame, CV_GAUSSIAN, 3, 3);

			//converting the original image into grayscale
			IplImage* imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3);
			cvCvtColor(frame, imgHSV, CV_BGR2HSV);

			IplImage* imgThresh = GetThresholdedImage(imgHSV);
			cvSmooth(imgThresh, imgThresh, CV_GAUSSIAN, 3, 3);



			//track the possition of the ball
			arrowObject(imgThresh, &xdirecv[toggler], &ydirecv[toggler]);
			xmarkObject(imgThresh, &myAngles[toggler]);

			if (toggler == 9){

				for (int idx = 0; idx < 10; idx++){
					xdirecval = xdirecval + xdirecv[idx];
					ydirecval = ydirecval + ydirecv[idx];
				}

				xdirecval = xdirecval / 10;
				ydirecval = ydirecval / 10;

				if ((xdirecval == 0) && (ydirecval == 0)){
					myAngle = 'f';
				}
				else{
					if (xdirecval == 0){
						if (ydirecval > 0){
							y_ratio = 1;
						}
						else{
							y_ratio = -1;
						}
						
						x_ratio = 0;
					}
					else if (ydirecval == 0){
						if (xdirecval > 0){
							x_ratio = 1;
						}
						else{
							x_ratio = -1;
						}
						y_ratio = 0;
					}
					else{
						if (xdirecval > 0){
							y_ratio = ydirecval / xdirecval;
							if (ydirecval > 0){
								x_ratio = xdirecval / ydirecval;
							}
							else{
								x_ratio = xdirecval / (-ydirecval);
							}
						}
						else{
							y_ratio = ydirecval / (-xdirecval);
							if (ydirecval > 0){
								x_ratio = xdirecval / ydirecval;
							}
							else{
								x_ratio = xdirecval / (-ydirecval);
							}
						}
						
						
					}

					if (y_ratio < 0){
						myAngle = 'b';
					}
					else{
						if (x_ratio < 0){
							myAngle = 'l';
						}
						else if (x_ratio>0){
							myAngle = 'r';
						}
						else{
							myAngle = 'g';
						}
					}
				}
				
			
					


				


				
				for (int idx = 0; idx < 10; idx++){
					if (myAngles[idx] == 's'){
						cnt = cnt + 1;
					}
				}
				if (cnt>4){
					myAngle = 's';
				}

			}


			// Add the tracking image and the frame
			cvAdd(frame, imgTracking, frame);

			cvShowImage("Video", frame);
			cvShowImage("Ball", imgThresh);


			//Clean up used images
			cvReleaseImage(&imgThresh);
			cvReleaseImage(&frame);
			cvReleaseImage(&imgHSV);

			//Sleep(100);



			toggler = toggler + 1;
			//printf("toggler is %d\n", toggler);
		}
		toggler = 0;



		printf("x : %d\n", xdirecval);
		printf("y : %d\n", ydirecval);
		printf("mark is %c\n", myAngle);
		printf("cnt is %d\n", cnt);
		printf("x_ratio is %d\n", x_ratio);
		printf("y_ratio is %d\n", y_ratio);

		cnt = 0;
		
		xdirecval = 0;
		ydirecval = 0;
		
		for (int idx = 0; idx < 10; idx++){
			myAngles[idx] = 0;
			xdirecv[idx] = 0;
			ydirecv[idx] = 0;
		}


		myAngle = 'f';
		SP->WriteData(&myAngle, 1);  // angle?
		//Sleep(500);
		SP->ReadData(&direc, 1);
		printf("dis1 is %d\n", direc);
		SP->ReadData(&direc, 1);
		printf("dis2 is %d\n", direc);
		SP->ReadData(&direc, 1);
		printf("dis3 is %d\n", direc);
		



	}
	return 0;
}