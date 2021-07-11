#pragma once
//#include <opencv2/highgui.hpp>
//#include <opencv2/opencv.hpp>
//#include <string>
//#include <vector>
#include "DataController.h"

using namespace std;

class Map
{
private:
	cv::Mat img;
	cv::Mat paint_square(const int&, const int&);
public:
	Map::Map();
	cv::Mat show(const vector<Paint_Unit>&,const int&,const int&);
	cv::Mat relocate(const int&, const int&);
};
