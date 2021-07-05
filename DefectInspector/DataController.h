#pragma once
#include <Windows.h>
#include <windows.system.h>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
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
	//ignore
	void create_floder();
	bool map_change = false;
	int filter_variable = 0;//決定篩選條件的變數，暫時先這樣
	//color function
	cv::Scalar decide_color_roi(const data_unit*);//decide the color of piex at  ROI
	cv::Scalar decide_color_map(const double&);//decide the color of piex at map
	//新的
	//統計相關的結構和變數
	typedef struct Statistics_node
	{
		int node_count_level_0[100] = { 0 };
		int node_count_level_1[10000] = { 0 };
		Statistics_node() {}
		Statistics_node(const int& x, const int& y) { node_count_level_0[(y / 1000) * 10 + (x / 1000)]++; node_count_level_1[(y / 100) * 100 + (x / 100)]++; }
		void insert(const int& x, const int& y);//插入資料
		int search_statistics(const int& x, const int& y);//回傳統計資料，輸入位置(此為給level_0用的)
		int search_statistics(const int& x0, const int& y0, const int& x1, const int& y1);//回傳統計資料，輸入位置(此為給level_1用的)
	}statistics_node;
	map<string, statistics_node> statistics_map;//map種類的變數
	void insert_statistics(const int& x, const int& y, const data_unit* input_data);//把新資料加入map中
	//分隔
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
	//新的
	/*取得統計資料的函式*/
	int get_statistics_data(const int& x, const int& y, const string& target);//取得目標類別的統計資料for level 1
	int get_statistics_data(const int& x0, const int& y0, const int& x1, const int& y1, const string& target);//取得目標類別的統計資料for level 2 (x0 代表level 0的x座標，x1 代表level 1的x座標，以此類推)
	int get_statistics_data(const int& x0, const int& y0, const int& x1, const int& y1, const int& x2, const int& y2, const string& target);//取得目標類別的統計資料for level 3
	//分隔
	DataControlUnit()
	{
		index = new data_unit ** [10000];
		for (int i = 0; i < 10000; i++)
		{
			index[i] = new data_unit * [10000]();
		}
	}
};
