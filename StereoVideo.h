////////////// Hybrid ���� ////////////////////////
///////////// Final Version //////////////////////
/////////////Video Extract 3D points ////////////////
/////////// ������ 010-6427-7905 ////////////
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
//USE_FUNC == 0 ->reprojectImageTo3D �Լ� ���
//USE_FUNC == 1 ->�������
//DIVIDE == 0 -> Image ũ�� �״��
//DIVIDE == 1 -> Image ũ�� ��
/*
Ŭ���� ��� ������ ����� ��Ÿ���� ǥ���� m�� ǥ������ ������
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
	//�̹��� ������ ����
	Size img_size;
	int Method;
	//Text ���� �о� ���� ����
	FILE* Flute;
	FILE* Violin;
	FILE* Cello;
	//Text ���Ͽ� �����ؾ��� ����
	FILE* Out_F;
	FILE* Out_V;
	FILE* Out_C;	
	// ī�޶� ���� �ܺ� �Ķ����
	Mat Q;
	Mat R, T, R1, P1, R2, P2;
	Mat M1, D1, M2, D2;
	//
	//Rectification ����
	Mat map11, map12, map21, map22;
	//
	Mat img1, img2;
	char keyboard;
	//��� ��Ī
	StereoBM bm;
	StereoSGBM sgbm;
	Mat disp, disp8, disp16;
	//���ְ���� ���� �����ؾ� �� ����
	FILE * Angle_F;
	FILE * Angle_C;
	FILE * Angle_V;
};
#else
#endif