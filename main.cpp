#include <string>
#include <iostream>
#include <stdio.h>
#include "IntImage.h"
#include "CascadeClassifier.h"
#include "ocv.h"

using namespace std;

int main()
{
	CascadeClassifier face;
	string filename = "./data/cascade.txt";
	face.LoadDefaultCascade(filename);
	cvNamedWindow("face",CV_WINDOW_AUTOSIZE);

	IntImage img;
	CvCapture* capture = cvCreateCameraCapture(0);

	if(capture == NULL)
	{
		fprintf(stderr,"error capture is null!\n");
		return -1;
	}

	IplImage* frame ;
	string imageName = "./9.bmp";

	while(1)
	{
		frame = cvQueryFrame(capture);
		if(!frame)
		{
			break;
		}
#if 0
		cvSaveImage(imageName.c_str(),frame);
		img.Load(imageName.c_str());
#endif
		img.IplImageToIntImage(frame);
		face.ApplyOriginalSize(img,"./data/cascade.txt.range");
		//img.Save(imageName);
		//cvShowImage("face",cvLoadImage(imageName.c_str(),0));
		cvShowImage("face",img.IntImageToIplImage());
		char c = cvWaitKey(10);
		if(c == 27)
		{
		    break;
		}
	}	

	return 0;
}
