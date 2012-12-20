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
	printf("count:%d\n",face.count);
	IntImage img;
	FILE* output  = fopen("./out.txt","a");
	cvNamedWindow("face",CV_WINDOW_AUTOSIZE);
	CvCapture* capture = cvCreateCameraCapture(0);
	if(capture == NULL)
	{
		fprintf(output,"error capture is null!\n");
		return -1;
	}
	fclose(output);
	IplImage* frame ;
	string imageName = "./9.bmp";
	//string imageName2 = "/home/andrew/8.bmp";
	while(1)
	{
		frame = cvQueryFrame(capture);
		if(!frame)
		{
			break;
		}
		cvSaveImage(imageName.c_str(),frame);
		img.Load(imageName.c_str());
		face.ApplyOriginalSize(img,"./data/cascade.txt.range");
		img.Save(imageName);
		cvShowImage("face",cvLoadImage(imageName.c_str(),0));
		//cvShowImage("face",frame);
		char c = cvWaitKey(10);
	}	

    return 0;
}
