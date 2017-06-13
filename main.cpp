#include "StereoCalib.h"


int main()
{
	//Chessboard ���� ���� 
	Size boardSize(7, 4);  /// 5,4 /// ���� ���� ���� �����ϴ°� 
	//input���� �� stereo image�� �̸��� ����� xml
	string imagelistfn = "stereo_calib.xml";
	bool showRectified = true;
	//stereo image �̸��� ������ ����
	vector<string> imagelist;
	bool ok = readStringList(imagelistfn, imagelist);
	cout << "������ ũ��(= image ����) : " << imagelist.size() << endl;
	StereoCalib(imagelist, boardSize, true, showRectified);
	return 0;
}