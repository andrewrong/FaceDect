#include <string>
#include <iostream>
#include <stdio.h>
#include <vector>
#include "IntImage.h"
#include "MyStruct.h"
#include "CascadeClassifier.h"
#include "ocv.h"

using namespace std;

IplImage* CamShift(IplImage* frame,const MRect& rect);

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
	IplImage* tmpImage;
	vector<MRect> results;
	bool detected = 0;
	while(1)
	{
	    frame = cvQueryFrame(capture);
	    if(!frame)
	    {
		    break;
	    }

	    if(!detected)
	    {
		img.IplImageToIntImage(frame);
		face.ApplyOriginalSize(img,results);
		if(results.size() != 0)
		{
		    detected = 1;
		    for(int i = 0; i < results.size();i++)
		    {
			cvRectangle(frame,cvPoint(results[i].left,results[i].top),cvPoint(results[i].right,results[i].bottom),cvScalar(0x00,0x00,0xff));
		    }
		    printf("rect's num is %d,(%ld,%ld),(%ld,%ld)\n",results.size(),results[0].left,results[0].top,results[0].right,results[0].bottom);
		}
		cvShowImage("face",frame);
	    }

	    if(detected)
	    {
		tmpImage = CamShift(frame,results[0]);
		cvShowImage("face",tmpImage);
		//cvReleaseImage(&tmpImage);
	    }

	    char c = cvWaitKey(10);
	    if(c == 27)
	    {
		break;
	    }
	}	
	cvReleaseCapture(&capture);
	cvDestroyWindow("face");
	return 0;
}
