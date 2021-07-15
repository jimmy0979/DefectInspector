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

enum Data_type {
	AllDefeat = 0, NormalDies
};

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
	
	//curr loaction at different level
	int level_0_x = 0, level_0_y = 0;
	int level_1_x = 0, level_1_y = 0;
	int level_2_x = 0, level_2_y = 0;
	//the number of defeat at different level
	int count_level_0[100] = {0};
	int count_level_1[10000] = {0};
	int count_level_2[1000000] = {0};
	
	void create_floder();//ignore

	//varible about setting
	bool map_change = false;//indicate while map block's location changing if whole map needs changing  
	int level = 0;//curr level
	Data_type filter_variable = AllDefeat;//the variable decide show which type's die
	//color function
	cv::Scalar decide_color_roi(const data_unit*);//decide the color of piex at  ROI
	cv::Scalar decide_color_map(const double&);//decide the color of piex at map

	// realTime infoList
	string infoList = "";

	//statistics 
	typedef struct Statistics_node//the struct to store specific die type's quantity at different level
	{
		int node_count_level_0[100] = { 0 };
		int node_count_level_1[10000] = { 0 };
		Statistics_node() {}
		Statistics_node(const int& x, const int& y) { node_count_level_0[(y / 1000) * 10 + (x / 1000)]++; node_count_level_1[(y / 100) * 100 + (x / 100)]++; }
		void insert(const int& x, const int& y);//insert data. it will automatically plus 1 to corresponding location 
		int search_statistics(const int& x, const int& y);//return the number of specific type at corresponding location at level 0
		int search_statistics(const int& x0, const int& y0, const int& x1, const int& y1);//return the number of specific type at corresponding location at level 1
	}statistics_node;
	map<int, statistics_node> statistics_map;//the object managing our statistics_node. 
	void insert_statistics(const int& x, const int& y, const data_unit* input_data);//insert new data in map

public:
	const int return_level(void) { return level;}
	//System::String return_jpgname(const int&, const vector<__int64>&, const vector<string>&);//ignore
	void put_data(const int&, const int&, const int&, const string&);//take data from sqlcommuncator into datacontroller(with bincode input)
	void put_data(const int&, const int&, const int&);//(no bincode for test)
	vector<paint_unit> pull_data(const bool&);
	bool update_data(const int& , const int&);
	bool change_level(const bool&);
	bool change_block(const int&);
	bool change_filter(const int&);//change the type of dies you can visible, if return false mean doesn't change type
	const Data_type return_fliter_setting(void);
	bool return_map_change(void);
	const int return_locat_x(void);
	const int return_locat_y(void);

	int return_lotId(void);
	pair<int, int> return_locat_xy(int, int);

	string currentInfoList(void);
	map<int, int> return_defect_count(void);

	int get_statistics_data(const int& x, const int& y, const int& target);//return the number of specific type at corresponding location at level 0
	int get_statistics_data(const int& x0, const int& y0, const int& x1, const int& y1, const int& target);//return the number of specific type at corresponding location at level 1
	int get_statistics_data(const int& x0, const int& y0, const int& x1, const int& y1, const int& x2, const int& y2, const int& target);//return the number of specific type at corresponding location at level 2

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
