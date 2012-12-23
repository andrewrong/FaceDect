#include "ocv.h"
#include "MyStruct.h"

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

IplImage* CamShift(IplImage* frame,const MRect& rect)
{
    selection.x = rect.left;
    selection.y = rect.top;
    selection.width = rect.right - rect.left;
    selection.height = rect.bottom - rect.top;

    if( !image )
    {
        /* allocate all the buffers */
        image = cvCreateImage( cvGetSize(frame), 8, 3 );
        image->origin = frame->origin;
        hsv = cvCreateImage( cvGetSize(frame), 8, 3 );
        hue = cvCreateImage( cvGetSize(frame), 8, 1 );
        mask = cvCreateImage( cvGetSize(frame), 8, 1 );
        backproject = cvCreateImage( cvGetSize(frame), 8, 1 );
        hist = cvCreateHist( 1, &hdims, CV_HIST_ARRAY, &hranges, 1 );
    }
    cvCopy(frame,image,0);
    cvCvtColor(image,hsv,CV_BGR2HSV);

    int _vmin = vmin, _vmax = vmax;

    cvInRangeS(hsv, cvScalar(0,smin,MIN(_vmin,_vmax),0),
                        cvScalar(180,256,MAX(_vmin,_vmax),0), mask );
    cvSplit( hsv, hue, 0, 0, 0 );

    float max_val = 0.f;

    if(track_object)
    {
	cvSetImageROI(hue, selection);
    	cvSetImageROI(mask, selection);
    	cvCalcHist(&hue, hist, 0, mask);
    	cvGetMinMaxHistValue(hist, 0, &max_val, 0, 0);
    	cvConvertScale(hist->bins,hist->bins,max_val ? 255. / max_val : 0., 0 );
    	cvResetImageROI(hue);
    	cvResetImageROI(mask);
    	track_window = selection;
	track_object = 0;
    }

    cvCalcBackProject(&hue, backproject, hist);
    cvAnd( backproject, mask, backproject, 0 );
    cvCamShift( backproject, track_window,
                cvTermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ),
                &track_comp, &track_box );
    track_window = track_comp.rect;

    if(!image->origin )
    {
        track_box.angle = -track_box.angle;
        cvEllipseBox(image, track_box, CV_RGB(255,0,0), 3, CV_AA, 0 );
    }
    return image;
}

