/*
 * describe:矩阵旋转
 * author: andrewrong
 * */
#include <iostream>
#include <string>
#include <fstream>
#include "opencv/cv.h"
#include "opencv/highgui.h"

using namespace std;

void MatrixRotate(int x,int y,float degree,int& rotatedX,int& rotatedY)
{
    float map[2];
    map[0] = x;
    map[1] = y;

    CvMat mat = cvMat(2,1,CV_32F,map);

    double angle = (degree * CV_PI) / 180.0;
    double sina = sin(angle);
    double cosa = cos(angle);

    float mulMap[4];
    CvMat mulMat = cvMat(2,2,CV_32F,mulMap);
    mulMap[0] = cosa;
    mulMap[1] = sina;
    mulMap[2] = -sina;
    mulMap[3] = cosa;

    float dst[2] = {0};
    CvMat dstMat = cvMat(2,1,CV_32F,dst);

    cvGEMM(&mulMat,&mat,1,NULL,0,&dstMat,0);
    
    rotatedX = dst[0];
    rotatedY = dst[1];
}
