////////////// Hybrid 과제 ////////////////////////
///////////// Final Version //////////////////////
/////////////Video Extract 3D points ////////////////
/////////// 정현조 010-6427-7905 ////////////
////////// guswh7905@kw.ac.kr ///////////////


#ifndef STEREOVIDEO__H_
#define STEREOVIDEO__H_

#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv/cv.h>
#include <opencv/cxmisc.h>
#include <opencv/highgui.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctype.h>
#include <stdlib.h>
#include <iterator>
#include <stdio.h>
#include <math.h>

using namespace std;
using namespace cv;

#define METHOD 1
#define USE_FUNC 0
#define DIVIDE 0
#define PI 3.141592
//METHOD == STEREO_BM == 0
//METHOD == STEREO_SGBM == 1
//USE_FUNC == 0 ->reprojectImageTo3D 함수 사용
//USE_FUNC == 1 ->직접계산
//DIVIDE == 0 -> Image 크기 그대로
//DIVIDE == 1 -> Image 크기 반
/*
클래스 멤버 변수는 멤버를 나타내는 표식자 m을 표시하자 앞으로
*/
class VIDEO{
public:
	void ReadYML(char *intrinsic_filename, char *extrinsic_filename);
	void ShowVideo(char *img_L, char *img_R, int Method);	
	void OpenReadTxt(char *Flute, char *Cello, char *Violin);
	void OpenWriteCoordinate(char *Out_Flute, char *Out_Cello, char *Out_Violin);
	void OpenWriteAngle(char *A_Flute, char *C_Flute, char *V_Flute);
	void CloseTxt();
private:
	void Extract3DandAngle();
	//int rows;
	//int cols;
	//이미지 사이즈 변수
	Size img_size;
	int Method;
	//Text 파일 읽어 오는 변수
	FILE* Flute;
	FILE* Violin;
	FILE* Cello;
	//Text 파일에 저장해야할 변수
	FILE* Out_F;
	FILE* Out_V;
	FILE* Out_C;	
	// 카메라 내부 외부 파라미터
	Mat Q;
	Mat R, T, R1, P1, R2, P2;
	Mat M1, D1, M2, D2;
	//
	//Rectification 변수
	Mat map11, map12, map21, map22;
	//
	Mat img1, img2;
	char keyboard;
	//블록 매칭
	StereoBM bm;
	StereoSGBM sgbm;
	Mat disp, disp8, disp16;
	//광주과기원 측에 전달해야 할 각도
	FILE * Angle_F;
	FILE * Angle_C;
	FILE * Angle_V;
};
#else
#endif