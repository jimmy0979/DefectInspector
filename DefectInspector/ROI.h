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
public:
	ROI();
	cv::Mat show(const vector<paint_unit>&,const int&);
	bool seach_jpg(const string&);//searh stored img
};
