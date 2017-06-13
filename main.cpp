#include "StereoCalib.h"


int main()
{
	//Chessboard 가로 세로 
	Size boardSize(7, 4);  /// 5,4 /// 잊지 말고 여기 수정하는거 
	//input으로 들어갈 stereo image의 이름이 저장된 xml
	string imagelistfn = "stereo_calib.xml";
	bool showRectified = true;
	//stereo image 이름을 저장할 벡터
	vector<string> imagelist;
	bool ok = readStringList(imagelistfn, imagelist);
	cout << "벡터의 크기(= image 개수) : " << imagelist.size() << endl;
	StereoCalib(imagelist, boardSize, true, showRectified);
	return 0;
}