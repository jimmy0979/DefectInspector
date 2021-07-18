#include "Map.h"

using namespace std;
using namespace cv;

//定義map的長寬
#define map_W 240
#define map_L 480
//定義map的切分數量
#define divide 10

//map的建構子
Map::Map()
{
	Map::img = Mat(Size(map_W, map_L), CV_8UC3,Scalar(0,255,0));//初始化新的陣列
}

//更改篩選條件時，更改方框顏色及底色
void Map::change_background(const Data_type& type)
{
	switch (type)
	{
	case Data_type::AllDefeat:
		SquareColor = cv::Scalar(255, 255, 255);
		background = cv::Scalar(0, 255, 0);
		break;
	case Data_type::NormalDies:
		SquareColor = cv::Scalar(255, 0, 0);
		background = cv::Scalar(255, 255, 255);
		break;
	}
}

//繪製目前選取位置的方框，並以不同顏色區別
Mat Map::paint_square(const int& x, const int& y)
{
	for (int i = 0; i < 10; i++)//重新繪製區隔的方框
	{
		for (int j = 0; j < 10; j++)
		{
			rectangle(this->img, Rect(j * (map_W / divide), i * (map_L / divide), map_W / divide, map_L / divide), Scalar(0, 0, 0));
		}
	}
	rectangle(this->img, Rect(x * (map_W / divide), y * (map_L / divide), map_W / divide, map_L / divide), this->SquareColor);//繪製被選擇區域的方框
	return this->img;
}

//繪製map
Mat Map::show(const vector< Paint_Unit>& data,const int& x=0,const int& y=0)
{
	Map::img = background;//繪製底色
	for (int i = 0; i < data.size(); i++)//繪製相對應區塊的顏色
		rectangle(Map::img, Rect(data[i].paintx * map_W/divide, data[i].painty * map_L/divide, map_W/divide, map_L/divide), data[i].color, -1);
	for (int i = 0; i < 10; i++)//繪製區隔的方框
	{
		for (int j = 0; j < 10; j++)
		{
			rectangle(img, Rect(j * (map_W / divide), i * (map_L / divide), map_W / divide, map_L / divide), Scalar(0, 0, 0));
		}
	}
	rectangle(Map::img, Rect(x * (map_W / divide), y * (map_L / divide), map_W / divide, map_L / divide), this->SquareColor);//paint selected square
	return Map::img;
}

//當更改選取位置時，重新繪製選取的方框
Mat Map::relocate(const int& x, const int& y)
{
	return paint_square(x,y);
}