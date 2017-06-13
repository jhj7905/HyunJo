////////////// Hybrid 과제 ////////////////////////
///////////// Camera Calibration ////////////////////
//////////// 좌 우 영상의 이름이 저장된 xml을 읽어서 matrices 계산//////////////////
/////////// 정현조 010-6427-7905 ////////////
////////// guswh7905@kw.ac.kr ///////////////



#ifndef STEREOCALIB_H_
#define STEREOCALIB_H_
#include <opencv\cv.h>
#include <opencv\cxmisc.h>
#include <opencv\highgui.h>
#include <opencv2\calib3d\calib3d.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <ctype.h>
#include <stdlib.h>
#include <iterator>
#include <stdio.h>

using namespace std;
using namespace cv;

void StereoCalib(const vector<string>& imagelist, Size boardSize, bool useCalibrated, bool showRectified);
bool readStringList(const string& filename, vector<string>& l);
#else
#endif