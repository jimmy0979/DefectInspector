#pragma once
#include "DataController.h"

using namespace std;

class Map
{
private:
	cv::Mat img;//繪製用的陣列
	cv::Mat paint_square(const int&, const int&);//繪製目前選取位置的方框，並以不同顏色區別
public:
	Map::Map();//map的建構子
	cv::Mat show(const vector<Paint_Unit>&,const int&,const int&);//繪製map
	cv::Mat relocate(const int&, const int&);//當更改選取位置時，重新繪製選取的方框
};
