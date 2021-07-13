#include "Map.h"

using namespace std;
using namespace cv;

#define map_W 240
#define map_L 480
#define divide 10

Map::Map()
{
	Map::img = Mat(Size(map_W, map_L), CV_8UC3,Scalar(0,255,0));
}

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

Mat Map::paint_square(const int& x, const int& y)
{
	for (int i = 0; i < 10; i++)//paint square
	{
		for (int j = 0; j < 10; j++)
		{
			rectangle(this->img, Rect(j * (map_W / divide), i * (map_L / divide), map_W / divide, map_L / divide), Scalar(0, 0, 0));
		}
	}
	rectangle(this->img, Rect(x * (map_W / divide), y * (map_L / divide), map_W / divide, map_L / divide), this->SquareColor);//paint selected square
	return this->img;
}

Mat Map::show(const vector< Paint_Unit>& data,const int& x=0,const int& y=0)
{
	Map::img = background;
	for (int i = 0; i < data.size(); i++)
		rectangle(Map::img, Rect(data[i].paintx * map_W/divide, data[i].painty * map_L/divide, map_W/divide, map_L/divide), data[i].color, -1);
	for (int i = 0; i < 10; i++)//paint square
	{
		for (int j = 0; j < 10; j++)
		{
			rectangle(img, Rect(j * (map_W / divide), i * (map_L / divide), map_W / divide, map_L / divide), Scalar(0, 0, 0));
		}
	}
	rectangle(Map::img, Rect(x * (map_W / divide), y * (map_L / divide), map_W / divide, map_L / divide), this->SquareColor);//paint selected square
	return Map::img;
}

Mat Map::relocate(const int& x, const int& y)
{
	return paint_square(x,y);
}