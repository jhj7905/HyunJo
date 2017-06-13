#include "StereoMat.h"
// Reproject image to 3D
Stereo::Stereo(char *img_L, char *img_R, int Option){
	this->Option = Option;
	const char* img1_filename = img_L;
	const char* img2_filename = img_R;
	IplImage *IMG1 = cvLoadImage(img1_filename, 0);
	IplImage *IMG2 = cvLoadImage(img2_filename, 0);
	Mat Img1(IMG1);
	Mat Img2(IMG2);
	rows = IMG1->height;
	cols = IMG1->width;
	if (this->Option == 0){  
		//resize(Img1, img1, Size(cols, rows));  // 동영상 처리 위해
		//resize(Img2, img2, Size(cols, rows));
		img1 = Img1;
		img2 = Img2;
		cout << "원본이미지" << endl;
		cout << "이미지의 크기 : " << rows << "\t" << cols << endl;
	}
	else{
		resize(Img1, img1, Size(cols / 2, rows / 2));
		resize(Img2, img2, Size(cols / 2, rows / 2));
		cout << "이미지 가로 세로 2분의 1" << endl;
		cout << "이미지의 크기 : " << img1.rows << "\t" << img1.cols << endl;
	}
}
void Stereo::ReadYML(char *intrinsic_filename, char *extrinsic_filename){
	if (intrinsic_filename){
		// reading intrinsic parameters
		FileStorage fs(intrinsic_filename, CV_STORAGE_READ);
		if (!fs.isOpened()){
			printf("Failed to open file %s\n", intrinsic_filename);
		}
		if (this->Option == 0){
			Mat M1, D1, M2, D2;
			fs["M1"] >> M1;
			fs["D1"] >> D1;
			fs["M2"] >> M2;
			fs["D2"] >> D2;
			// reading extrinsic parameters
			fs.open(extrinsic_filename, CV_STORAGE_READ);
			if (!fs.isOpened()){
				printf("Failed to open file %s\n", extrinsic_filename);
			}
			Mat R, T, R1, P1, R2, P2;
			fs["R"] >> R;
			fs["T"] >> T;
			fs["R1"] >> R1;
			fs["R2"] >> R2;
			fs["P1"] >> P1;
			fs["P2"] >> P2;
			fs["Q"] >> Q;
			Mat map11, map12, map21, map22;
			initUndistortRectifyMap(M1, D1, R1, P1, Size(cols, rows), CV_16SC2, map11, map12); //1440,810
			initUndistortRectifyMap(M2, D2, R2, P2, Size(cols, rows), CV_16SC2, map21, map22);
			Mat img1r, img2r;
			remap(img1, img1r, map11, map12, INTER_LINEAR);
			remap(img2, img2r, map21, map22, INTER_LINEAR);
			img1 = img1r;
			img2 = img2r;

			fs.release();
			//Baseline 계산 Tx
			double baseline = sqrtf(powf(T.at<double>(0, 0), 2.0) + powf(T.at<double>(1, 0), 2.0) + powf(T.at<double>(2, 0), 2.0));
			cout << "Baseline 길이 : " << baseline << "cm" << endl;
		}
		else{
			Mat M1, D1, M2, D2;
			fs["M1"] >> M1;
			// 영상 크기를 가로 세로 2분의 1씩 줄였기 때문에 fx,fy,cx,cy도 각각 2분의 1로 줄이는 형태
			M1.at<double>(0, 0) /= 2.0;
			M1.at<double>(0, 2) /= 2.0;
			M1.at<double>(1, 1) /= 2.0;
			M1.at<double>(1, 2) /= 2.0;
			fs["D1"] >> D1;
			fs["M2"] >> M2;
			M2.at<double>(0, 0) /= 2.0;
			M2.at<double>(0, 2) /= 2.0;
			M2.at<double>(1, 1) /= 2.0;
			M2.at<double>(1, 2) /= 2.0;
			fs["D2"] >> D2;
			// reading extrinsic parameters
			fs.open(extrinsic_filename, CV_STORAGE_READ);
			if (!fs.isOpened()){
				printf("Failed to open file %s\n", extrinsic_filename);
			}
			Mat R, T, R1, P1, R2, P2;
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
			Mat map11, map12, map21, map22;
			initUndistortRectifyMap(M1, D1, R1, P1, Size(cols / 2, rows / 2), CV_16SC2, map11, map12); //1440,810
			initUndistortRectifyMap(M2, D2, R2, P2, Size(cols / 2, rows / 2), CV_16SC2, map21, map22);
			Mat img1r, img2r;
			remap(img1, img1r, map11, map12, INTER_LINEAR);
			remap(img2, img2r, map21, map22, INTER_LINEAR);
			img1 = img1r;
			img2 = img2r;
			//imshow("left", img1);
			//imshow("right", img2);
			fs.release();
			//Baseline 계산 Tx
			double baseline = sqrtf(powf(T.at<double>(0, 0), 2.0) + powf(T.at<double>(1, 0), 2.0) + powf(T.at<double>(2, 0), 2.0));
			cout << "Baseline 길이 : " << baseline << "cm" << endl;
		}
	}
}
void Stereo::StereoParameter(int Method){
	this->Method = Method;
	//15,128  // 15,160  // 17, 160으로 한 형태에서 조정
	int SADWindowSize = 17, numberOfDisparities = 80; // SADWindowSize는 홀수로, numberOfDisparities는 16의 배수로 들어가야한다
	if (this->Method == 0){		
		bm.state->preFilterSize = 9;
		bm.state->preFilterCap = 31;  //31
		bm.state->SADWindowSize = SADWindowSize;
		bm.state->minDisparity = 0;  //-64
		bm.state->numberOfDisparities = numberOfDisparities;
		bm.state->textureThreshold = 5;
		bm.state->uniquenessRatio = 0;
		bm.state->speckleWindowSize = 11;
		bm.state->speckleRange = 32;
	//	bm.state->disp12MaxDiff = 1;
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
		//sgbm.fullDP = true; //false
		sgbm(img1, img2, disp);
	}
	
	//waitKey(0);
	//disp - Output disparity map. it is a 16-bit signed single-channel image of the same size as the input image
	//it contains disparity values scaled by 16. so to get the floating point disparity map,
	//you need to divide each disp element by 16
	disp.convertTo(disp8, CV_32F, (1.0) / (16.0));
	disp.convertTo(disp16, CV_8U, (255.0) / ((double)numberOfDisparities*16.0));
	Mat xyz(disp8.size(), CV_32FC3);
	reprojectImageTo3D(disp8, xyz, Q, false, CV_32F);
	saveXYZ("3D.txt", xyz);

	imshow("left", img1);
	imshow("right", img2);
	imshow("disparity_disp", disp);
	imshow("disparity_disp16", disp16);
	imwrite("output.jpg", disp16);

	waitKey();
	//return disp8;
}
void saveXYZ(const char* filename, const Mat& mat)
{
	const double max_z = 1.0e4;
	FILE* fp = fopen(filename, "wt");
	for (int y = 0; y < mat.rows; y++){  // 여기에 cvRect 변수 넣어서 하면될듯..
		for (int x = 0; x < mat.cols; x++){
			Vec3f point = mat.at<Vec3f>(y, x);
			if (fabs(point[2] - max_z) < FLT_EPSILON || fabs(point[2]) > max_z) continue;
			fprintf(fp, "%4d %4d\t %f %f %f\n", x, y, point[0], point[1], point[2]);
		}
	}
	fclose(fp);
}



void customReproject(Mat& disparity, Mat& Q, Mat& out3D)  // 810 , 1440 이미지 크기(rows,cols)
{
	//CV_Assert(disparity.type() == CV_32F && !disparity.empty());
	//CV_Assert(Q.type() == CV_32F && Q.cols == 4 && Q.rows == 4);

	// 3-channel matrix for containing the reprojected 3D world coordinates
	out3D = Mat::zeros(disparity.size(), CV_32FC3);
	// Getting the interesting parameters from Q, everything else is zero or one
	double Q03, Q13, Q23, Q32, Q33;
	Q03 = Q.at<double>(0, 3);
	Q13 = Q.at<double>(1, 3);
	Q23 = Q.at<double>(2, 3);
	Q32 = Q.at<double>(3, 2);  // 단위 조심해서 넣기
	Q33 = Q.at<double>(3, 3);
	cout << "Q(0,3) = " << Q03 << "; Q(1,3) = " << Q13 << "; Q(2,3) = " << Q23 << "; Q(3,2) = " << Q32 << "; Q(3,3) = " << Q33 << ";" << endl;
	// Transforming a single-channel disparity map to a 3-channel image representing a 3D surface
	for (int i = 0; i < disparity.rows; i++)
	{
		//uchar* disp_ptr = disparity.ptr<uchar>(i);
		Vec3f* out3D_ptr = out3D.ptr<Vec3f>(i);

		for (int j = 0; j < disparity.cols; j++)
		{
			float d = disparity.at<float>(i, j);  // 들어갈 때 타입 생각잘하기   
			//uchar d = disparity.at<uchar>(i, j);
			//uchar d = disp_ptr[j];
			if (d == 0.f) /////////////  if (d == 0.0f)  if (d == 0.f || d >= 720) ////////////////////////////////
				continue;  //Discard bad pixels   
			float pw = 1.0f / (d * Q32 + Q33);
			Vec3f& point = out3D_ptr[j];
			point[0] = (static_cast<float>(j)+Q03) * pw;
			point[1] = (static_cast<float>(i)+Q13) * pw;
			point[2] = Q23 * pw;
		}
	}
	cout << "Escape from getting XYZ" << endl;
}

