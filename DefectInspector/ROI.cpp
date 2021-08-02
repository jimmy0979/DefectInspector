#include "ROI.h"

using namespace std;
using namespace cv;


ROI::ROI()//ROI的建構子
{
	ROI::img = Mat(Size(1000, 1000), CV_8UC3,Scalar(0,255,0));
}


//搜尋已儲存的圖片(未加入功能)
bool ROI::seach_jpg(const string &target)
{
	if (haveImageReader(target))
	{
		ROI::img = imread(target);
		return true;
	}
	else
		return false;
}

//繪製ROI
Mat ROI::show(const vector<paint_unit> &data, const int &level)
{
	ROI::img = cv::Scalar(255,255,255);//繪製底色
	int width;
	switch (level) {//根據不同level時，設定繪製方寬框的邊長
	case 0:
		width = 1;
		break;
	case 1:
		width = 10;
		break;
	case 2:
		width = 100;
	}
	for(int i=0;i<data.size();i++)//繪製Dies
		rectangle(img, Rect(data[i].paintx * width, data[i].painty * width, width, width),data[i].color,-1);
	return img;
}