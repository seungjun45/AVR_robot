
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
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




int main(){
    //load the video file to the memory
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

    //iterate through each frames of the video     
    while(true){

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

        //Wait 10mS
        int c = cvWaitKey(10);
        //If 'ESC' is pressed, break the loop
        if((char)c==27 ) break; 
		printf("%d",xdirecval);
		printf("%d\n",ydirecval);
    }

    cvDestroyAllWindows();
    cvReleaseImage(&imgTracking);
    cvReleaseCapture(&capture);     

    return 0;
}



/*
#include <cv.h>
#include <highgui.h>

IplImage* imgTracking;
int lastX = -1;
int lastY = -1;

//This function threshold the HSV image and create a binary image
IplImage* GetThresholdedImage(IplImage* imgHSV){       
    IplImage* imgThresh=cvCreateImage(cvGetSize(imgHSV),IPL_DEPTH_8U, 1);
    cvInRangeS(imgHSV, cvScalar(0,160,60), cvScalar(30,2556,256), imgThresh); 
    return imgThresh;
}

void trackObject(IplImage* imgThresh){
    // Calculate the moments of 'imgThresh'
    CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
    cvMoments(imgThresh, moments, 1);
    double moment10 = cvGetSpatialMoment(moments, 1, 0);
    double moment01 = cvGetSpatialMoment(moments, 0, 1);
    double area = cvGetCentralMoment(moments, 0, 0);

     // if the area<1000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
    if(area>1000){
        // calculate the position of the ball
        int posX = moment10/area;
        int posY = moment01/area;        
        
       if(lastX>=0 && lastY>=0 && posX>=0 && posY>=0)
        {
            // Draw a yellow line from the previous point to the current point
            cvLine(imgTracking, cvPoint(posX, posY), cvPoint(lastX, lastY), cvScalar(0,0,255), 4);
        }

         lastX = posX;
        lastY = posY;
    }

     free(moments); 
}


int main(){
   
      CvCapture* capture =0;       
      capture = cvCaptureFromCAM(0);
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
     cvNamedWindow("Ball");

      //iterate through each frames of the video     
      while(true){

            frame = cvQueryFrame(capture);           
            if(!frame) break;
            frame=cvCloneImage(frame); 
            
            cvSmooth(frame, frame, CV_GAUSSIAN,3,3); //smooth the original image using Gaussian kernel

            IplImage* imgHSV = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3); 
            cvCvtColor(frame, imgHSV, CV_BGR2HSV); //Change the color format from BGR to HSV
            IplImage* imgThresh = GetThresholdedImage(imgHSV);
          
            cvSmooth(imgThresh, imgThresh, CV_GAUSSIAN,3,3); //smooth the binary image using Gaussian kernel
            
           //track the possition of the ball
           trackObject(imgThresh);

            // Add the tracking image and the frame
           cvAdd(frame, imgTracking, frame);

            cvShowImage("Ball", imgThresh);           
           cvShowImage("Video", frame);
           
           //Clean up used images
           cvReleaseImage(&imgHSV);
           cvReleaseImage(&imgThresh);            
           cvReleaseImage(&frame);

            //Wait 10mS
            int c = cvWaitKey(10);
            //If 'ESC' is pressed, break the loop
            if((char)c==27 ) break;      
      }

      cvDestroyAllWindows() ;
      cvReleaseImage(&imgTracking);
      cvReleaseCapture(&capture);     

      return 0;
}*/