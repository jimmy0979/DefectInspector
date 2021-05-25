#pragma once
#include <Windows.h>
#include <windows.system.h>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

using namespace std;

typedef struct Paint_Unit
{
	int paintx, painty;
	cv::Scalar color;
	Paint_Unit(int x, int y, cv::Scalar c) :paintx(x), painty(y), color(c) {}
}paint_unit;

class DataControlUnit
{
private:
	typedef	struct Data_Unit
	{
		int diex, diey;
		int defeat_type;
		string bin_code;//5 digts
		Data_Unit(const int& x,const int& y,const int& defeat) :diex(x), diey(y), defeat_type(defeat){}
		Data_Unit(const int& x,const int& y,const int& defeat,const string& bin) :diex(x), diey(y),defeat_type(defeat),bin_code(bin){}
	}data_unit;
	data_unit** index = nullptr;
	int level = 0;
	//curr loaction at different level¡Y©Òµ•Ø≈
	int level_0_x = 0, level_0_y = 0;
	int level_1_x = 0, level_1_y = 0;
	int level_2_x = 0, level_2_y = 0;
	//temp varible
	int count_level_0[100] = {0};
	int count_level_1[10000] = {0};
	int count_level_2[1000000] = {0};
	//ignore
	void create_floder();
	bool map_change = false;
public:
	const int return_level(void) { return level;}
	//System::String return_jpgname(const int&, const vector<__int64>&, const vector<string>&);//ignore
	void put_data(const int&, const int&, const int&, const string&);//take data from sqlcommuncator into datacontroller(with bincode input)
	void put_data(const int&, const int&, const int&);//(no bincode for test)
	vector<paint_unit> pull_data(const bool&);
	bool change_level(const bool&);
	bool change_block(const int&);
	bool return_map_change(void);
	const int return_locat_x(void);
	const int return_locat_y(void);
	DataControlUnit() { index = new data_unit * [100000000]; };
};
