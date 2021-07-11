#include "Map.h"

using namespace std;
using namespace cv;

#define map_W 240
#define map_L 480

Map::Map()
{
	Map::img = Mat(Size(map_W, map_L), CV_8UC3,Scalar(0,255,0));
}

Mat Map::paint_square(const int& x, const int& y)
{
	Mat temp = Mat(Size(map_W, map_L), CV_8UC3);
	temp = img.clone();
	int divide = 10;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			rectangle(temp, Rect(j * (map_W / divide), i * (map_L / divide), map_W / divide, map_L / divide), Scalar(0, 0, 0));
		}
	}
	rectangle(temp, Rect(x * (map_W / divide), y * (map_L / divide), map_W / divide, map_L / divide), Scalar(255,255,255));
	return temp;
}

Mat Map::show(const vector< Paint_Unit>& data,const int& x=0,const int& y=0)
{
	Map::img = Scalar(0,255,0);
	int divide = 10;
	for (int i = 0; i < data.size(); i++)
		rectangle(Map::img, Rect(data[i].paintx * map_W/divide, data[i].painty * map_L/divide, map_W/divide, map_L/divide), data[i].color, -1);
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			rectangle(img, Rect(j * (map_W / divide), i * (map_L / divide), map_W / divide, map_L / divide), Scalar(0, 0, 0));
		}
	}
	rectangle(Map::img, Rect(x * (map_W / divide), y * (map_L / divide), map_W / divide, map_L / divide), Scalar(255, 255, 255));
	return Map::img;
}

Mat Map::relocate(const int& x, const int& y)
{
	return paint_square(x,y);
}