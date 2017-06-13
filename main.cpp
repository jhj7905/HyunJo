#include "StereoMat.h"


int main()
{
	// 810 , 1440 �̹��� ũ��(rows,cols) 
	// Stereo Calibration Solution���κ��� ����� matrices
	char* intrinsic_filename = "C:/Users/JHJ/Documents/Visual Studio 2013/Projects/HybridAudio/GetXML/Output/intrinsics_gwangju.yml";
	char* extrinsic_filename = "C:/Users/JHJ/Documents/Visual Studio 2013/Projects/HybridAudio/GetXML/Output/extrinsics_gwangju.yml";
	// Input ����
	char* img1_filename = "Input/LL.JPG";   // "L (33).JPG"
	char* img2_filename = "Input/RR.JPG";   //"R(33).JPG"
	// char* disparity_filename = "disprity.jpg";
	Stereo Stereo(img1_filename, img2_filename, 1);   /// 1�� ������ 0�� �����̹���
	Stereo.ReadYML(intrinsic_filename, extrinsic_filename);
	//Mat disp8;
	// 0�� bm 1�� sgbm
	Stereo.StereoParameter(0);	
	return 0;
}

