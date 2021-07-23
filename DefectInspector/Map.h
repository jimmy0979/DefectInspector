#pragma once
#include "DataController.h"

using namespace std;

class Map
{
private:
	cv::Mat img;//繪製用的陣列
	cv::Scalar background = cv::Scalar(0,255,0);//設定背景顏色的變數，預設綠色
	cv::Scalar SquareColor = cv::Scalar(255, 255, 255);//設定選取方框顏色的變數，預設白色
	cv::Mat paint_square(const int&, const int&);//繪製目前選取位置的方框，並以不同顏色區別
public:
	Map::Map();//map的建構子
	void change_background(const Data_type&);//更改篩選條件時，更改方框顏色及底色
	cv::Mat show(const vector<Paint_Unit>&,const int&,const int&);//繪製map
	cv::Mat relocate(const int&, const int&);//當更改選取位置時，重新繪製選取的方框
};
