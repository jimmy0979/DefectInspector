#pragma once
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include "DataController.h"

using namespace std;

class Map
{
private:
	cv::Mat img;
	void paint_line(const int&);
	cv::Mat paint_square(const int&, const int&);//show witch one we select now
public:
	Map::Map();
	cv::Mat show(const vector<Paint_Unit>&,const int&,const int&);
	cv::Mat relocate(const int&, const int&, const int&);//user select another block
};
