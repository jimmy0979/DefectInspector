#pragma once
#include "DataController.h"

using namespace std;

class ROI
{
private:
	cv::Mat img;//繪製用的陣列
public:
	ROI();//ROI的建構子
	cv::Mat show(const vector<paint_unit>&,const int&);//繪製ROI
	bool seach_jpg(const string&);//搜尋已儲存的圖片(未加入功能)
};
