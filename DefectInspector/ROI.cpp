#include "ROI.h"

using namespace std;
using namespace cv;


ROI::ROI()
{
	ROI::img = Mat(Size(1000, 1000), CV_8UC3,Scalar(0,255,0));
}

void ROI::change_background(const Data_type& type)
{
	switch (type)
	{
	case Data_type::AllDefeat:
		background = cv::Scalar(0, 255, 0);
		break;
	case Data_type::NormalDies:
		background = cv::Scalar(255, 255, 255);
		break;
	}
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
	ROI::img = background;
	int width;
	switch (level) {
	case 0:
		width = 1;
		break;
	case 1:
		width = 10;
		break;
	case 2:
		width = 100;
	}
	for(int i=0;i<data.size();i++)
		rectangle(img, Rect(data[i].paintx * width, data[i].painty * width, width, width),data[i].color,-1);
	return img;
}