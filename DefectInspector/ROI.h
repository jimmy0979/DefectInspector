#pragma once
//#include <opencv2/highgui.hpp>
//#include <opencv2/opencv.hpp>
//#include <string>
//#include <vector>
#include "DataController.h"

using namespace std;

class ROI
{
private:
	cv::Mat img;
	int ROI_W, ROI_L;
public:
	ROI(const int&,const int&);
	cv::Mat show(const vector<paint_unit>&,const int&);
	bool seach_jpg(const string&);//searh stored img
};
