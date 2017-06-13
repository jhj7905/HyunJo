#include "StereoVideo.h"

//bar에서 사용할 Global 변수
int g_minDisparity = 0;
int g_SADWindowSize = 5;  // 범위가 5~255라서 초기화를 일단 5로 시작함 (홀수가 들어가야한다)
int g_textureThreshold = 0;
int g_uniquenessRatio = 0;
int g_numberOfDisparities = 16; // 16의 배수로 들어가야 한다
int g_preFilterSize = 5; // 범위가 5~255라서 초기화를 일단 5로 시작함 (홀수가 들어가야한다)
int g_preFilterCap = 1;  // 범위가 1~63
int g_speckleWindowSize = 0;
int g_speckleRange = 0;

void VIDEO::OpenReadTxt(char *Flute, char *Cello, char *Violin){
	this->Flute = fopen(Flute, "rt");
	if (!this->Flute)
		cout << "Flute txt 파일이 없습니다" << endl;
	this->Cello = fopen(Cello, "rt");
	if (!this->Cello)
		cout << "Cello txt 파일이 없습니다" << endl;
	this->Violin = fopen(Violin, "rt");
	if (!this->Violin)
		cout << "Violin txt 파일이 없습니다" << endl;
	return;
}
void VIDEO::OpenWriteCoordinate(char *Out_Flute, char *Out_Cello, char *Out_Violin){
	Out_F = fopen(Out_Flute, "wt");
	Out_C = fopen(Out_Cello, "wt");
	Out_V = fopen(Out_Violin, "wt");
	return;
}
void VIDEO::OpenWriteAngle(char *A_Flute, char *C_Flute, char *V_Flute){
	Angle_F = fopen(A_Flute, "wt");
	Angle_C = fopen(C_Flute, "wt");
	Angle_V = fopen(V_Flute, "wt");
	return;
}

void VIDEO::CloseTxt(){
	fclose(Flute);
	fclose(Violin);
	fclose(Cello);
	fclose(Out_F);
	fclose(Out_V);
	fclose(Out_C);
	fclose(Angle_C);
	fclose(Angle_F);
	fclose(Angle_V);
	return;
}
void VIDEO::ReadYML(char *intrinsic_filename, char *extrinsic_filename){
	if (intrinsic_filename)
	{
		// reading intrinsic parameters		
		FileStorage fs(intrinsic_filename, CV_STORAGE_READ);
		if (!fs.isOpened())
		{
			printf("Failed to open file %s\n", intrinsic_filename);
		}
		fs["M1"] >> M1;
		fs["D1"] >> D1;
		fs["M2"] >> M2;
		fs["D2"] >> D2;
		M1.at<double>(0, 0) /= 2.0;
		M1.at<double>(0, 2) /= 2.0;
		M1.at<double>(1, 1) /= 2.0;
		M1.at<double>(1, 2) /= 2.0;
	
		M2.at<double>(0, 0) /= 2.0;
		M2.at<double>(0, 2) /= 2.0;
		M2.at<double>(1, 1) /= 2.0;
		M2.at<double>(1, 2) /= 2.0;	

		fs.open(extrinsic_filename, CV_STORAGE_READ);
		if (!fs.isOpened())
		{
			printf("Failed to open file %s\n", extrinsic_filename);			
		}
		fs["R"] >> R;
		fs["T"] >> T;
		fs["R1"] >> R1;
		fs["R2"] >> R2;
		fs["P1"] >> P1;
		fs["P2"] >> P2;
		fs["Q"] >> Q;
		P1.at<double>(0, 0) /= 2.0;
		P1.at<double>(0, 2) /= 2.0;
		P1.at<double>(1, 1) /= 2.0;
		P1.at<double>(1, 2) /= 2.0;

		P2.at<double>(0, 0) /= 2.0;
		P2.at<double>(0, 2) /= 2.0;
		P2.at<double>(1, 1) /= 2.0;
		P2.at<double>(1, 2) /= 2.0;

		Q.at<double>(0, 3) /= 2.0;
		Q.at<double>(1, 3) /= 2.0;
		Q.at<double>(2, 3) /= 2.0;
		Q.at<double>(3, 3) /= 2.0;

		fs.release();
	}
	initUndistortRectifyMap(M1, D1, R1, P1, Size(1440 / 2, 810 / 2), CV_16SC2, map11, map12);
	initUndistortRectifyMap(M2, D2, R2, P2, Size(1440 / 2, 810 / 2), CV_16SC2, map21, map22);
	return ;
}
void VIDEO::ShowVideo(char *img_L, char *img_R, int Method){
	const char* Video_L = img_L;
	const char* Video_R = img_R;
	CvCapture* Left = cvCreateFileCapture(Video_L);
	CvCapture* Right = cvCreateFileCapture(Video_R);
	IplImage* frame_L;
	IplImage* frame_R;
	Mat img1r, img2r;
	keyboard = 0;
	while (1){
		frame_L = cvQueryFrame(Left);
		frame_R = cvQueryFrame(Right);
		if (!frame_L || !frame_R)
			break;
		Mat IMG1(frame_L);
		Mat IMG2(frame_R);
		cout << IMG1.size() << endl;

		resize(IMG1, img1, Size(1440 / 2, 810 / 2));
		resize(IMG2, img2, Size(1440 / 2, 810 / 2));
		cvtColor(img1, img1, CV_BGR2GRAY);
		cvtColor(img2, img2, CV_BGR2GRAY);
		img_size = img1.size();
		remap(img1, img1r, map11, map12, INTER_LINEAR);
		remap(img2, img2r, map21, map22, INTER_LINEAR);
		img1 = img1r;
		img2 = img2r;
		//Rectification 보기 위한 imshow //
		/*imshow("Rectification_Left", img1);
		imshow("Rectification_Right", img2);*/
		this->Method = Method;
		//15,128  // 15,160  // 17, 160으로 한 형태에서 조정
		// SADWindowSize는 홀수로, numberOfDisparities는 16의 배수로 들어가야한다
		int SADWindowSize = 9, numberOfDisparities = 48; 
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////한번만 초기화하면되는데
		///////////////////////////////////계속 하고있네
		if (this->Method == 0){
			////이 부분이 연산을 빠르게 할 수 있는 ROI 역할을 하지 않을까 생각
			//bm.state->roi1 = roi1;
			//bm.state->roi2 = roi2;
			//실험 통해서 확인해보기
			bm.state->preFilterSize = 5;  //범위는 0~255
			bm.state->preFilterCap = 31;  
			bm.state->SADWindowSize = SADWindowSize;
			bm.state->minDisparity = 0;  //-64
			bm.state->numberOfDisparities = numberOfDisparities;
			bm.state->textureThreshold = 5; //10
			bm.state->uniquenessRatio = 5; //15
			bm.state->speckleWindowSize = 9;
			bm.state->speckleRange = 32;		
			bm.state->uniquenessRatio = 0;
			//namedWindow("controller", 1); // 변수들 Control 하기 위해서
			//cvResizeWindow("controller", 512, 512);
			//createTrackbar("Spe_Win", "controller", &g_speckleWindowSize, 200, NULL);
			//bm.state->speckleWindowSize = g_speckleWindowSize;

			//createTrackbar("Spe_Ran", "controller", &g_speckleRange, 200, NULL);
			//bm.state->speckleRange = g_speckleRange;

			//createTrackbar("Pre_FS", "controller", &g_preFilterSize, 91, NULL); //255까지지만 91로 설정
			//if (g_preFilterSize < 5)
			//	bm.state->preFilterSize = 5;
			//else{
			//	if (g_preFilterSize % 2 == 0)
			//		bm.state->preFilterSize = g_preFilterSize + 1;
			//	else
			//		bm.state->preFilterSize = g_preFilterSize;
			//}
			//createTrackbar("Pre_FC", "controller", &g_preFilterCap, 63, NULL);
			//if (g_preFilterCap < 1)
			//	bm.state->preFilterCap = 1;
			//else
			//	bm.state->preFilterCap = g_preFilterCap;

			//createTrackbar("Min_Dis", "controller", &g_minDisparity, 100, NULL);
			//bm.state->minDisparity = g_minDisparity;

			//createTrackbar("NUM_Di", "controller", &g_numberOfDisparities, 300, NULL);
			//if (g_numberOfDisparities < 16)
			//	bm.state->numberOfDisparities = numberOfDisparities = 16;
			//else{
			//	if (g_numberOfDisparities % 16 == 0)
			//		bm.state->numberOfDisparities = numberOfDisparities = g_numberOfDisparities;
			//	else
			//		bm.state->numberOfDisparities = numberOfDisparities = (g_numberOfDisparities / 16) * 16;
			//}
			//createTrackbar("SAD_Win", "controller", &g_SADWindowSize, 91, NULL); //255까지지만 91로 설정
			//if (g_SADWindowSize < 5)
			//	bm.state->SADWindowSize = 5;
			//else{
			//	if (g_SADWindowSize % 2 == 0)
			//		bm.state->SADWindowSize = g_SADWindowSize + 1;
			//	else
			//		bm.state->SADWindowSize = g_SADWindowSize;
			//}
			//createTrackbar("Text_Th", "controller", &g_textureThreshold, 100, NULL);
			//bm.state->textureThreshold = g_textureThreshold;

			//createTrackbar("Uniq_Ra", "controller", &g_uniquenessRatio, 100, NULL);
			//bm.state->uniquenessRatio = g_uniquenessRatio;
			bm(img1, img2, disp);
		}
		else{
			sgbm.preFilterCap = 20; //31
			sgbm.SADWindowSize = SADWindowSize;
			int cn = img1.channels();
			sgbm.P1 = 8 * cn*sgbm.SADWindowSize*sgbm.SADWindowSize;
			sgbm.P2 = 32 * cn*sgbm.SADWindowSize*sgbm.SADWindowSize;
			sgbm.minDisparity = 0;
			sgbm.numberOfDisparities = numberOfDisparities;
			sgbm.uniquenessRatio = 0;
			sgbm.speckleWindowSize = 11;
			sgbm.speckleRange = 32;
			sgbm.disp12MaxDiff = 1;
			sgbm.fullDP = true; //false
			//sgbm.fullDP = true; //false
			//namedWindow("controller", 1); // 변수들 Control 하기 위해서
			//cvResizeWindow("controller", 512, 512);
			//createTrackbar("Spe_Win", "controller", &g_speckleWindowSize, 200, NULL);
			//sgbm.speckleWindowSize = g_speckleWindowSize;

			//createTrackbar("Spe_Ran", "controller", &g_speckleRange, 200, NULL);
			//sgbm.speckleRange = g_speckleRange;

			//createTrackbar("Pre_FC", "controller", &g_preFilterCap, 63, NULL);
			//if (g_preFilterCap < 1)
			//	sgbm.preFilterCap = 1;
			//else
			//	sgbm.preFilterCap = g_preFilterCap;

			//createTrackbar("Min_Dis", "controller", &g_minDisparity, 100, NULL);
			//sgbm.minDisparity = g_minDisparity;

			//createTrackbar("NUM_Di", "controller", &g_numberOfDisparities, 300, NULL);
			//if (g_numberOfDisparities < 16)
			//	sgbm.numberOfDisparities = numberOfDisparities = 16;
			//else{
			//	if (g_numberOfDisparities % 16 == 0)
			//		sgbm.numberOfDisparities = numberOfDisparities = g_numberOfDisparities;
			//	else
			//		sgbm.numberOfDisparities = numberOfDisparities = (g_numberOfDisparities / 16) * 16;
			//}
			//createTrackbar("SAD_Win", "controller", &g_SADWindowSize, 91, NULL); //255까지지만 91로 설정
			//if (g_SADWindowSize < 5)
			//	sgbm.SADWindowSize = 5;
			//else{
			//	if (g_SADWindowSize % 2 == 0)
			//		sgbm.SADWindowSize = g_SADWindowSize + 1;
			//	else
			//		sgbm.SADWindowSize = g_SADWindowSize;
			//}
			//createTrackbar("Uniq_Ra", "controller", &g_uniquenessRatio, 100, NULL);
			//sgbm.uniquenessRatio = g_uniquenessRatio;
			sgbm(img1, img2, disp);
		}
		disp.convertTo(disp8, CV_32F, (1.0) / (16.0));
		disp.convertTo(disp16, CV_8U, (255.0) / ((double)numberOfDisparities*16.0));	
		/// Extract 함수 들어갈 자리
		Extract3DandAngle();
		//printf("\n");
		
		imshow("left", img1);
		imshow("right", img2);
		//imshow("disparity_disp", disp);
		imshow("disparity_disp16", disp16);
		keyboard = waitKey(3); //  keyboard = waitKey(30);
		if (keyboard == 27)
			break; // 27 == ESC
		if (keyboard == 32){ // 32 == Space key -> pause
			while ((keyboard = waitKey(10)) != 32 && keyboard != 27);
			if (keyboard == 27)
				break;
		}	
		
	}
	cvReleaseCapture(&Left);
	cvReleaseCapture(&Right);
	return ;
}
void VIDEO::Extract3DandAngle(){
	// Tracking 결과로 저장된 x,y 좌표 읽어 들일 좌표 변수
	Point F_coordinate;
	Point V_coordinate;
	Point C_coordinate;
	// Tracking 결과로 저장된 x,y 좌표 읽어 들이는 부분
	fscanf(Flute,"%d %d\n", &F_coordinate.x, &F_coordinate.y);
	fscanf(Violin,"%d %d\n", &V_coordinate.x, &V_coordinate.y);
	fscanf(Cello,"%d %d\n", &C_coordinate.x, &C_coordinate.y);
	// 3차원 좌표 구할때 사용될 외부 행렬 (Extrinsic)
	double Q03, Q13, Q23, Q32, Q33;
	Q03 = Q.at<double>(0, 3);
	Q13 = Q.at<double>(1, 3);
	Q23 = Q.at<double>(2, 3);
	Q32 = Q.at<double>(3, 2);  // 단위 조심해서 넣기
	Q33 = Q.at<double>(3, 3);

	//////////////////
	float d_V = disp8.at<float>(V_coordinate.y, V_coordinate.x);
	float d_F = disp8.at<float>(F_coordinate.y, F_coordinate.x);
	float d_C = disp8.at<float>(C_coordinate.y, C_coordinate.x);
	//cout << F_coordinate.x << "\t" << F_coordinate.y << endl;

	float pw_V = 1.0f / (d_V * Q32 + Q33);
	float pw_F = 1.0f / (d_F * Q32 + Q33);
	float pw_C = 1.0f / (d_C * Q32 + Q33);

	Vec3f point_V;
	point_V[0] = (static_cast<float>(V_coordinate.x) + Q03) * pw_V;
	point_V[1] = (static_cast<float>(V_coordinate.y) + Q13) * pw_V;
	point_V[2] = Q23 * pw_V;

	Vec3f point_F;
	point_F[0] = (static_cast<float>(F_coordinate.x) + Q03) * pw_F;
	point_F[1] = (static_cast<float>(F_coordinate.y) + Q13) * pw_F;
	point_F[2] = Q23 * pw_F;

	Vec3f point_C;
	point_C[0] = (static_cast<float>(C_coordinate.x) + Q03) * pw_C;
	point_C[1] = (static_cast<float>(C_coordinate.y) + Q13) * pw_C;
	point_C[2] = Q23 * pw_C;

	//if문으로 무한대값 및 잘못된 값 처리하기 
	fprintf(Out_V, "%f %f %f\n", point_V[0], point_V[1], point_V[2]);
	fprintf(Out_F, "%f %f %f\n", point_F[0], point_F[1], point_F[2]);
	fprintf(Out_C, "%f %f %f\n", point_C[0], point_C[1], point_C[2]);

	//카메라와 마이크로폰 어레이 Z값  //
	float distance = 370;
	//////////////////////////////////////
	float ANG_V = tan(point_V[0] / (point_V[2] - distance))*180.0 / PI;
	float ANG_F = tan(point_F[0] / (point_F[2] - distance))*180.0 / PI;
	float ANG_C = tan(point_C[0] / (point_C[2] - distance))*180.0 / PI;

	fprintf(Angle_V, "%f\n", ANG_V);
	fprintf(Angle_F, "%f\n", ANG_F);
	fprintf(Angle_C, "%f\n", ANG_C);
	return;
}
