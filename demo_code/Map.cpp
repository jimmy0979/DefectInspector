#include "Map.h"

using namespace std;
using namespace cv;

#define map_W 240
#define map_L 480

Map::Map()
{
	Map::img = Mat(Size(map_W, map_L), CV_8UC3,Scalar(0,255,0));
}


void Map::paint_line(const int &divide)
{
	for (int i = 1; i < divide; i++)//horizontal
	{
		line(Map::img, Point(i*map_W/divide,0), Point(i * map_W / divide,map_L), Scalar(0, 0, 0));
	}
	for (int i = 1; i < divide; i++)//vertical
	{
		line(Map::img, Point(0, i * map_L / divide), Point(map_W, i * map_L / divide), Scalar(0, 0, 0));
	}
}

Mat Map::paint_square(const int& x, const int& y,const int& level, Mat origin_img)
{
	Mat temp;
	origin_img.copyTo(temp);
	int divide = 10;
	rectangle(temp, Rect(x * (map_W / divide), y * (map_L / divide), map_W / divide, map_L / divide), Scalar(255,0,0));
	return temp;
}

Mat Map::show(const vector< Paint_Unit>& data, const int& level,const int& x=0,const int& y=0)
{
	Map::img = Scalar(0,255,0);
	int divide = 10;
	for (int i = 0; i < data.size(); i++)
		rectangle(Map::img, Rect(data[i].paintx * map_W/divide, data[i].painty * map_L/divide, map_W/divide, map_L/divide), data[i].color, -1);
	Map::paint_line(divide);
	return paint_square(x,y,level,Map::img);
}

Mat Map::relocate(const int& x, const int& y, const int& level)
{
	return paint_square(x,y,level,Map::img);
}