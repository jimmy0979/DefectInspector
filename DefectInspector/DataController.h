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
	__int64 paintx, painty;
	cv::Scalar color;
	Paint_Unit(__int64 x, __int64 y, cv::Scalar c) :paintx(x), painty(y), color(c) {}
}paint_unit;

class DataControlUnit
{
private:
	typedef	struct Data_Unit
	{
		__int64 diex, diey;
		__int64 defeat_type;
		string bin_code;//5 digts
		struct Data_Unit* pre = nullptr;
		struct Data_Unit* next = nullptr;
		Data_Unit(const __int64& x,const __int64& y,const __int64& defeat) :diex(x), diey(y), defeat_type(defeat){}
		Data_Unit(const __int64& x,const __int64& y,const __int64& defeat,const string& bin) :diex(x), diey(y),defeat_type(defeat),bin_code(bin){}
	}data_unit;
	data_unit* index[1000000] = {nullptr};
	int level = 0;
	//curr loaction at different level¡Y©Òµ•Ø≈
	__int64 level_0_x = 0, level_0_y = 0;
	__int64 level_1_x = 0, level_1_y = 0;
	__int64 level_2_x = 0, level_2_y = 0;
	//temp varible
	__int64 count_level_0[100] = {0};
	__int64 count_level_1[10000] = {0};
	__int64 count_level_2[1000000] = {0};
	//ignore
	void create_floder();
public:
	const int return_level(void) { return level;}
	System::String return_jpgname(const int&, const vector<__int64>&, const vector<string>&);//ignore
	void put_data(const __int64&, const __int64&, const __int64&, const string&);//take data from sqlcommuncator into datacontroller(with bincode input)
	void put_data(const __int64&, const __int64&, const __int64&);//(no bincode for test)
	vector<paint_unit> pull_data(const bool&);
};
