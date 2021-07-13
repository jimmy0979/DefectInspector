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
	cv::Scalar background = cv::Scalar(0, 255, 0);//defeat background color is green
public:
	ROI();
	void change_background(const Data_type&);
	cv::Mat show(const vector<paint_unit>&,const int&);
	bool seach_jpg(const string&);//searh stored img
};
