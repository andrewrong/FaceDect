#if 0
#include "ocv.h"
#include "MyStruct.h"
#include <stdio.h>

IplImage *image = 0, *hsv = 0, *hue = 0, *mask = 0, *backproject = 0;
CvHistogram *hist = 0;
int i,c;
int vmin = 10, vmax = 256, smin = 30;
float hranges_arr[] = {0,180};
float* hranges = hranges_arr;
int hdims = 16;
CvRect selection;
CvRect track_window;
CvBox2D track_box;
CvConnectedComp track_comp;
int track_object = 1;

void CamShift(IplImage* frame,const MRect& rect,int resetCam)
{
    selection.x = rect.left;
    selection.y = rect.top;
    selection.width = rect.right - rect.left; 
    selection.height = rect.bottom - rect.top; 

    if(!hsv)
    {
        /* allocate all the buffers */
        //image = cvCreateImage( cvGetSize(frame), 8, 3 );
        //image->origin = frame->origin;
        hsv = cvCreateImage( cvGetSize(frame), 8, 3 );
        hue = cvCreateImage( cvGetSize(frame), 8, 1 );
        mask = cvCreateImage( cvGetSize(frame), 8, 1 );
        backproject = cvCreateImage( cvGetSize(frame), 8, 1 );
        hist = cvCreateHist( 1, &hdims, CV_HIST_ARRAY, &hranges, 1 );
    }

    //cvCopy(frame,image,0);
    cvCvtColor(frame,hsv,CV_BGR2HSV);

    int _vmin = vmin, _vmax = vmax;

    cvInRangeS(hsv, cvScalar(0,smin,MIN(_vmin,_vmax),0),
                        cvScalar(180,256,MAX(_vmin,_vmax),0), mask );
    cvSplit( hsv, hue, 0, 0, 0 );


    if(resetCam)
    {
	float max_val = 0.f;
	cvSetImageROI(hue, selection);
    	cvSetImageROI(mask, selection);
    	cvCalcHist(&hue, hist, 0, mask);
    	cvGetMinMaxHistValue(hist, 0, &max_val, 0, 0);
    	//cvConvertScale(hist->bins,hist->bins,max_val ? 255. / max_val : 0., 0 );
    	cvResetImageROI(hue);
    	cvResetImageROI(mask);
    	track_window = selection;
    }

    cvCalcBackProject(&hue, backproject, hist);
    cvAnd( backproject, mask, backproject, 0 );
    cvCamShift( backproject, track_window,
                cvTermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ),
                &track_comp, &track_box );
    track_window = track_comp.rect;

    cvEllipseBox(frame, track_box, CV_RGB(255,0,0), 3, CV_AA, 0);
    //cvSaveImage("test.bmp",image);
    //printf("angle:%f\n",track_box.angle);    
    //cvRectangle(image,cvPoint(track_window.x,track_window.y),cvPoint(track_window.x + track_window.width,track_window.y +  track_window.height * 3.0 / 4),cvScalar(0x00,0x00,0xff),3);
}
#endif


#include "ocv.h"
#include "MyStruct.h"
#include <stdio.h>

IplImage *hsv = 0, *hue = 0, *mask = 0, *backproject = 0, *histimg = 0;
CvHistogram *hist = 0;

int backproject_mode = 0;

CvRect selection;
CvRect track_window;
CvBox2D track_box;
CvConnectedComp track_comp;
int hdims = 16;
float hranges_arr[] = {0,180};
float* hranges = hranges_arr;
int vmin = 10, vmax = 256, smin = 30;

CvScalar hsv2rgb( float hux )
{
    int rgb[3], p, sector;
    static const int sector_data[][3]=
        {{0,2,1}, {1,2,0}, {1,0,2}, {2,0,1}, {2,1,0}, {0,1,2}};
    hux *= 0.033333333333333333333333333333333f;
    sector = cvFloor(hux);
    p = cvRound(255*(hux - sector));
    p ^= sector & 1 ? 255 : 0;

    rgb[sector_data[sector][0]] = 255;
    rgb[sector_data[sector][1]] = 0;
    rgb[sector_data[sector][2]] = p;

    return cvScalar(rgb[2], rgb[1], rgb[0],0);
}

float CamShift(IplImage* frame,const CvRect& rect,int resetCam)
{
        int i, bin_w, c;
        if(!hsv)
        {
            hsv = cvCreateImage( cvGetSize(frame), 8, 3 );
            hue = cvCreateImage( cvGetSize(frame), 8, 1 );
            mask = cvCreateImage( cvGetSize(frame), 8, 1 );
            backproject = cvCreateImage( cvGetSize(frame), 8, 1 );
            hist = cvCreateHist( 1, &hdims, CV_HIST_ARRAY, &hranges, 1 );
            histimg = cvCreateImage( cvSize(320,200), 8, 3 );
            cvZero( histimg );
        }

        cvCvtColor(frame, hsv, CV_BGR2HSV );

        int _vmin = vmin, _vmax = vmax;

        cvInRangeS(hsv, cvScalar(0,smin,MIN(_vmin,_vmax),0),cvScalar(180,256,MAX(_vmin,_vmax),0), mask );
        cvSplit(hsv, hue, 0, 0, 0 );

        if(resetCam == 1)
        {
            float max_val = 0.f;
            cvSetImageROI(hue, rect);
            cvSetImageROI(mask, rect);
            cvCalcHist(&hue, hist, 0, mask );
	    cvGetMinMaxHistValue( hist, 0, &max_val, 0, 0 );
            cvConvertScale( hist->bins, hist->bins, max_val ? 255. / max_val : 0., 0 );
            cvResetImageROI( hue );
            cvResetImageROI( mask );
            track_window = rect;
            cvZero( histimg );
            bin_w = histimg->width / hdims;
            for( i = 0; i < hdims; i++ )
            {
                int val = cvRound( cvGetReal1D(hist->bins,i)*histimg->height/255 );
                CvScalar color = hsv2rgb(i*180.f/hdims);
                cvRectangle( histimg, cvPoint(i*bin_w,histimg->height),
                                 cvPoint((i+1)*bin_w,histimg->height - val),
                                 color, -1, 8, 0 );
            }
        }

        cvCalcBackProject(&hue, backproject, hist);
        cvAnd(backproject, mask, backproject, 0 );
        cvCamShift( backproject, track_window,cvTermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ),&track_comp, &track_box );
        track_window = track_comp.rect;
	//printf("track_window:(%d,%d,%d,%d)\n",track_window.x,track_window.y,track_window.width,track_window.height);	

        if(!frame->origin )
                track_box.angle = -track_box.angle;
        cvEllipseBox(frame, track_box, CV_RGB(255,0,0), 3, CV_AA, 0 );
	
	//cvRectangle(image,cvPoint(track_window.x,track_window.y),cvPoint(track_window.x + track_window.width,track_window.y +  track_window.height),cvScalar(0x00,0x00,0xff),3);

        cvNamedWindow( "Histogram", 1 );
        cvShowImage( "Histogram", histimg );

	return track_box.angle;
}


