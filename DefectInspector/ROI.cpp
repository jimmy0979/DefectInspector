#include "ROI.h"

using namespace std;
using namespace cv;


ROI::ROI(const int& W, const int& L)
{
	ROI::ROI_W = W;
	ROI::ROI_L = L;
	ROI::img = Mat(Size(10000, 10000), CV_8UC3,Scalar(0,255,0));
}

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

Mat ROI::show(const vector<paint_unit> &data, const int &level)
{
	ROI::img = Scalar(0,255,0);
	int width;
	if (level == 0)
		width = 1;
	else if (level == 1)
		width = 10;
	else
		width = 100;
	for(int i=0;i<data.size();i++)
		rectangle(img, Rect(data[i].paintx * width, data[i].painty * width, width, width),data[i].color,-1);

	resize(img,img,Size(ROI_W,ROI_L), 0, 0, cv::INTER_LINEAR);
	return img;
}