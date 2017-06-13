#include "StereoMat.h"


int main()
{
	// 810 , 1440 이미지 크기(rows,cols) 
	// Stereo Calibration Solution으로부터 계산한 matrices
	char* intrinsic_filename = "C:/Users/JHJ/Documents/Visual Studio 2013/Projects/HybridAudio/GetXML/Output/intrinsics_gwangju.yml";
	char* extrinsic_filename = "C:/Users/JHJ/Documents/Visual Studio 2013/Projects/HybridAudio/GetXML/Output/extrinsics_gwangju.yml";
	// Input 영상
	char* img1_filename = "Input/LL.JPG";   // "L (33).JPG"
	char* img2_filename = "Input/RR.JPG";   //"R(33).JPG"
	// char* disparity_filename = "disprity.jpg";
	Stereo Stereo(img1_filename, img2_filename, 1);   /// 1은 반으로 0은 원본이미지
	Stereo.ReadYML(intrinsic_filename, extrinsic_filename);
	//Mat disp8;
	// 0은 bm 1은 sgbm
	Stereo.StereoParameter(0);	
	return 0;
}

