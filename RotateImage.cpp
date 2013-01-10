#include <string>
#include "opencv/cv.h"
#include "opencv/highgui.h"

using namespace std;

void RotateImage(IplImage* img,IplImage* imgRotate,float degree)
{  
    double angle = degree  * CV_PI / 180.0; // 弧度    
    double a = sin(angle), b = cos(angle);   
    int width = img->width;    
    int height = img->height;    
    //int width_rotate= int(height * fabs(a) + width * fabs(b));    
    //int height_rotate=int(width * fabs(a) + height * fabs(b));    
    //旋转数组map  
    // [ m0  m1  m2 ] ===>  [ A11  A12   b1 ]  
    // [ m3  m4  m5 ] ===>  [ A21  A22   b2 ]  
    float map[6];  
    CvMat map_matrix = cvMat(2, 3, CV_32F, map);    
    // 旋转中心  
    CvPoint2D32f center = cvPoint2D32f(width / 2, height / 2);    
    cv2DRotationMatrix(center, degree, 1.0, &map_matrix);    
    //map[2] += (width_rotate - width) / 2;    
    //map[5] += (height_rotate - height) / 2;    
    //对图像做仿射变换  
    //CV_WARP_FILL_OUTLIERS - 填充所有输出图像的象素。  
    //如果部分象素落在输入图像的边界外，那么它们的值设定为 fillval.  
    //CV_WARP_INVERSE_MAP - 指定 map_matrix 是输出图像到输入图像的反变换，  
    cvWarpAffine( img,imgRotate, &map_matrix, CV_INTER_LINEAR | CV_WARP_FILL_OUTLIERS, cvScalarAll(0));    
}  
