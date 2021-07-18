#pragma once
#include "DataController.h"

using namespace std;

class ROI
{
private:
	cv::Mat img;//繪製用的陣列
	cv::Scalar background = cv::Scalar(0, 255, 0);//設定背景顏色的變數，預設綠色
public:
	ROI();//ROI的建構子
	void change_background(const Data_type&);//更改篩選條件時，更改底色
	cv::Mat show(const vector<paint_unit>&,const int&);//繪製ROI
	bool seach_jpg(const string&);//搜尋已儲存的圖片(未加入功能)
};
