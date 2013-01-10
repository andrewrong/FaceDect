#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include "IntImage.h"
#include "MyStruct.h"
#include "CascadeClassifier.h"
#include "ocv.h"

using namespace std;

float CamShift(IplImage* frame,const CvRect& rect,int resetCam);
void RotateImage(IplImage* frame,IplImage* imgRotate,float degree);
void MatrixRotate(int x,int y,float degree,int& rotatedX,int& rotatedY);

int main()
{
	CascadeClassifier face;

	string filename = "./data/cascade.txt";
	string filenameRange = "./data/cascade.txt.range";

	face.LoadDefaultCascade(filename,filenameRange);
	cvNamedWindow("face",CV_WINDOW_AUTOSIZE);

	IntImage img;

	CvCapture* capture = cvCreateCameraCapture(0);
	if(capture == NULL)
	{
		fprintf(stderr,"error capture is null!\n");
		return -1;
	}

	IplImage* frame ;
	vector<MRect> results;
	CvRect detectedRect;
	int detected = 0;
	CvPoint** points = new CvPoint*[1];
	points[0] = new CvPoint[4];

	while(1)
	{
	    int resetCam = 0;
	    frame = cvQueryFrame(capture);
	    if(!frame)
	    {
		    break;
	    }
	     
	    img.IplImageToIntImage(frame);
	    face.ApplyOriginalSize(img,results);

	    if(results.size() != 0)
	    {
		detected = 1;
		for(int i = 0; i < results.size();i++)
		{
		    cvRectangle(frame,cvPoint(results[i].left,results[i].top),cvPoint(results[i].right,results[i].bottom),cvScalar(0x00,0x00,0xff),3);
		}
		printf("rect's num is %d,(%ld,%ld),(%ld,%ld)\n",results.size(),results[0].left,results[0].top,results[0].right,results[0].bottom);
		detectedRect.x = results[0].left;
		detectedRect.width = results[0].right - results[0].left;
		detectedRect.y = results[0].top;
		detectedRect.height = results[0].bottom - results[0].top;

		resetCam = 1;
	    }
	    

	    if(resetCam == 1)
	    {
		cout<<"reset Rect"<<endl;
		CamShift(frame,detectedRect,resetCam);
	    }
	    else if(detected == 1 && resetCam == 0)
	    {
		cout<<"人脸跟踪"<<endl;
		float degree = CamShift(frame,detectedRect,resetCam);

		if(degree < 0)
		{
		    cout<<"Head to the right"<<endl;
		    degree = fabs(degree) - 100;
		    cout<<"Rdegress: "<<degree<<endl;
		}
		else
		{
		    cout<<"Head to the left"<<endl;
		    degree = 100 - degree;
		    cout<<"Ldegress: "<<degree<<endl;
		}

		IplImage* imgRotate = cvCreateImage(cvGetSize(frame),8,3);
		RotateImage(frame,imgRotate,degree);
		img.IplImageToIntImage(imgRotate);
		face.ApplyOriginalSize(img,results);
		cout<<"num:"<<results.size()<<endl;
		if(results.size() != 0)
	    	{
	    	    for(int i = 0; i < results.size();i++)
	    	    {
	    	        cvRectangle(imgRotate,cvPoint(results[i].left,results[i].top),cvPoint(results[i].right,results[i].bottom),cvScalar(0xff,0x00,0x00),3);
	    	    }
	    	    printf("rect's num is %d,(%ld,%ld),(%ld,%ld)\n",results.size(),results[0].left,results[0].top,results[0].right,results[0].bottom);
	    	}
		
#if 0	
		if(results.size() > 0)
		{
		   int left = results[0].left;
		   int right = results[0].right;
		   int top = results[0].top;
		   int bottom = results[0].bottom;

		   cout<<"("<<left<<","<<top<<"),("<<right<<","<<bottom<<")"<<endl;
		   MatrixRotate(left,top,-degree,left,top);		
		   MatrixRotate(right,bottom,-degree,right,bottom);		
		    
		   cout<<"("<<left<<","<<top<<"),("<<right<<","<<bottom<<")"<<endl;
		   points[0][0] = cvPoint(left,top);
		   points[0][1] = cvPoint(right,top);
		   points[0][2] = cvPoint(left,bottom);
		   points[0][3] = cvPoint(right,bottom);

		   cout<<"hello"<<endl;
		   int pointNum[1] = {1};
		   cvPolyLine(frame,points,pointNum,1,1,CV_RGB(0,0,0),3);
		}
#endif
		cvCopy(imgRotate,frame);
	    }
	     
	    
	    cvShowImage("face",frame);

	    char c = cvWaitKey(10);
	    if(c == 27)
	    {
		break;
	    }
	    
#if 0
	    if(!detected)
	    {
		img.IplImageToIntImage(frame);
		face.ApplyOriginalSize(img,results);
		if(results.size() != 0)
		{
		    detected = 1;
		    for(int i = 0; i < results.size();i++)
		    {
			cvRectangle(frame,cvPoint(results[i].left,results[i].top),cvPoint(results[i].right,results[i].bottom),cvScalar(0x00,0x00,0xff),3);
		    }
		    printf("rect's num is %d,(%ld,%ld),(%ld,%ld)\n",results.size(),results[0].left,results[0].top,results[0].right,results[0].bottom);
		}
		cvShowImage("face",frame);
	    }

	    if(detected)
	    {
		tmpImage = CamShift(frame,results[0]);
		cvShowImage("face",tmpImage);
	    }

	    char c = cvWaitKey(10);
	    if(c == 27)
	    {
		break;
	    }
#endif
	}	
	delete[] points[0];
	delete[] points;
	cvReleaseCapture(&capture);
	cvDestroyWindow("face");
	return 0;
}
