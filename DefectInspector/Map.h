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
	cv::Scalar background = cv::Scalar(0,255,0);//default background's color is green
	cv::Scalar SquareColor = cv::Scalar(255, 255, 255);//default square's color is white
	cv::Mat paint_square(const int&, const int&);
public:
	Map::Map();
	void change_background(const Data_type&);
	cv::Mat show(const vector<Paint_Unit>&,const int&,const int&);
	cv::Mat relocate(const int&, const int&);
};
