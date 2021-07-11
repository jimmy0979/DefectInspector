#pragma once
#include <Windows.h>
#include <windows.system.h>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <map>
#include <utility>

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
	data_unit*** index;
	int level = 0;
	//curr loaction at different level
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
	int filter_variable = 0;//�M�w�z������ܼơA�Ȯɥ��o��
	//color function
	cv::Scalar decide_color_roi(const data_unit*);//decide the color of piex at  ROI
	cv::Scalar decide_color_map(const double&);//decide the color of piex at map

	// realTime infoList
	string infoList = "";

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

	int return_lotId(void);
	pair<int, int> return_locat_xy(int, int);

	string currentInfoList(void);
	map<int, int> return_defect_count(void);
	void test(void)
	{
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				cout << count_level_0[i * 10 + j] << '\n';
			}
		}
	}

	DataControlUnit()
	{
		index = new data_unit ** [10000];
		for (int i = 0; i < 10000; i++)
		{
			index[i] = new data_unit * [10000]();
		}
	}
};
