////////////// Hybrid 과제 ////////////////////////
///////////// obtain disparity map and 3D points  ////////////////////
//////////// matrices(카메라 내부행렬 외부 행렬)들이 저장된 intrinsics & extrinsics yml 파일을 읽어와서  
/////////// disparity map & 3D points
/////////// 정현조 010-6427-7905 ////////////
////////// guswh7905@kw.ac.kr ///////////////
//동영상으로 접근할 때 contour space pause 사용하기
/*



Depth map 좌표는 rectification 후의 좌영상의 좌표와 같다.

*/
#ifndef STEREOMAT_H_
#define STEREOMAT_H_

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv\cv.h>
#include <opencv\cxmisc.h>
#include <opencv\highgui.h>
#include <vector>
#include <string>
#include <algorithm>
#include <ctype.h>
#include <stdlib.h>
#include <iterator>
#include <stdio.h>
#include <iostream>
#include <opencv\cv.h>
#include <opencv\cxmisc.h>
#include <opencv\highgui.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctype.h>
#include <stdlib.h>
#include <iterator>
#include <stdio.h>
#include <math.h>

//http://stackoverflow.com/questions/22418846/reprojectimageto3d-in-opencv
//https://github.com/bkornel/Reproject-Image-To-3D/blob/master/main.cpp

using namespace std;
using namespace cv;
#define METHOD 1
#define USE_FUNC 0
#define DIVIDE 0
//METHOD == STEREO_BM == 0
//METHOD == STEREO_SGBM == 1
//USE_FUNC == 0 ->reprojectImageTo3D 함수 사용
//USE_FUNC == 1 ->직접계산
//DIVIDE == 0 -> Image 크기 그대로
//DIVIDE == 1 -> Image 크기 반
class Stereo{
public:
	friend void saveXYZ(const char* filename, const Mat& mat);
	Stereo(char *img_L, char *img_R, int Option);
	void ReadYML(char *intrinsic_filename, char *extrinsic_filename);
	void StereoParameter(int Method);

private:
	int Option;
	int rows;
	int cols;
	int Method;
	Mat img1, img2;
	Mat Q;
	Mat disp,disp8,disp16;
	StereoBM bm;
	StereoSGBM sgbm;
};

//void customReproject(Mat& disparity, Mat& Q, Mat& out3D);
//void saveXYZ(const char* filename, const Mat& mat);

#else
#endif