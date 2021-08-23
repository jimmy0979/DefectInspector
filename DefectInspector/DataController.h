#pragma once
#include <Windows.h>
#include <windows.system.h>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <queue>
#include <utility>

using namespace std;

//列舉所有的篩選條件
enum Data_type {
	Alldefect = 0, NormalDies
};

//繪圖的基本單元
typedef struct Paint_Unit
{
	int paintx, painty;//繪製的對應位置
	cv::Scalar color;//繪製的顏色
	Paint_Unit(int x, int y, cv::Scalar c) :paintx(x), painty(y), color(c) {}//Paint_Unit的建構子
}paint_unit;

class DataControlUnit
{
private:
	typedef	struct Data_Unit
	{
		int defect_type = -1;//-1代表沒有defect
		string bin_code;//5 digts
		Data_Unit() {}
		Data_Unit(const int& defect) : defect_type(defect){}
		Data_Unit(const int& defect,const string& bin) :defect_type(defect),bin_code(bin){}
		bool operator== (const struct Data_Unit& comp) const;//差bincode比對
		void operator=(const struct Data_Unit& b);
	}data_unit;
	data_unit*** index;
	
	//curr loaction at different level
	int level_0_x = 0, level_0_y = 0;
	int level_1_x = 0, level_1_y = 0;
	int level_2_x = 0, level_2_y = 0;
	//the number of defect at different level
	int count_level_0[100] = {0};
	int count_level_1[10000] = {0};
	int count_level_2[1000000] = {0};
	
	void create_floder();//ignore

	//varible about setting
	bool map_change = false;//indicate while map block's location changing if whole map needs changing  
	int level = 0;//curr level
	Data_type filter_variable = Alldefect;//the variable decide show which type's die
	//color function
	cv::Scalar decide_color_roi(const data_unit*);//decide the color of piex at  ROI
	cv::Scalar decide_color_map(const double&);//decide the color of piex at map

	// realTime infoList
	string infoList = "";

	/*跟統計資料相關*/
	typedef struct Statistics_node//儲存統計資料的結構。儲存等級0和1指定位置相對映類別的晶粒個數
	{
		int node_count_level_0[100] = { 0 };//儲存對映等級0位置的陣列
		int node_count_level_1[10000] = { 0 };//儲存對映等級1位置的陣列
		Statistics_node() {}
		Statistics_node(const int& x, const int& y) { node_count_level_0[(y / 1000) * 10 + (x / 1000)]++; node_count_level_1[(y / 100) * 100 + (x / 100)]++; }//建構子並且增加相對映的位置個數
		Statistics_node(const int& x, const int& y, const int& num) { node_count_level_0[(y / 1000) * 10 + (x / 1000)] += num; node_count_level_1[(y / 100) * 100 + (x / 100)] += num;}//建構子並且增加相對映的位置個數，並且個數可以自由決定
		void insert(const int& x, const int& y);//插入資料，會自動增加相對映位置的個數
		void insert(const int& x, const int& y, const int& num);//插入資料，會自動增加相對映位置的個數，並且個數可以自由決定
		bool minus(const int& x, const int& y);//使某key值指定位置統計數字減一，回傳布林值Ture代表減一成功
		int search_statistics(const int& x, const int& y);//回傳等級0指定位置的對映晶粒類別個數
		int search_statistics(const int& x0, const int& y0, const int& x1, const int& y1);////回傳等級1指定位置的對映晶粒類別個數
	}statistics_node;
	map<int, statistics_node> statistics_map;//使用STL的map管理
	void insert_statistics(const int& x, const int& y, const data_unit* input_data);//插入資料進入map中
	void insert_statistics(const int& x, const int& y, const data_unit* input_data, const int& num);//插入資料進入map中，並指定插入個數
	bool delete_statistics(const int& x, const int& y);//刪除絕對位置下的晶粒統計資料，並且回傳是否刪除成功 
	//跟統計的插入優化有關
	int last_locate_x = -1, last_locate_y = -1;
	int counts = 0;
	data_unit register_data = data_unit(-1);
	void classify_BFS(const int range_x,const int range_y);

public:
	
	//System::String return_jpgname(const int&, const vector<__int64>&, const vector<string>&);//ignore

	/*放入或更新陣列的資料*/
	void put_data(const int&, const int&, const int&, const string&);//take data from sqlcommuncator into datacontroller(with bincode input)
	void put_data(const int&, const int&, const int&);//(no bincode for test)
	void insert_done_reset();//插入資料結束，重置相關的變數

	/*取得目前狀態下的繪圖資訊*/
	vector<paint_unit> pull_data(const bool&);//true 取得ROI目前的繪圖資訊 false取得map目前的繪圖資訊

	bool update_data(const int& , const int&);

	/*變更狀態相關*/
	bool change_level(const bool&);//變更縮放等級，並且回傳是否變更成功
	bool change_block(const int&);//向指定方向位移一格。0向左 1向右 2向上 3向下。回傳是否位移成功
	bool move_block(const int& moveX, const int& moveY);//提供一個向量，格子依此向量方向移動
	bool return_map_change(void);//位移一格後，是否map需要更新。ture需要 false不需要
	bool change_filter(const int&);//變更篩選類別，並且回傳是否變更成功

	/*回傳當前狀態資訊*/
	const int return_level(void) { return level; }//回傳目前的縮放等級
	const int return_locat_x(void);//回傳目前x軸的相對位置
	const int return_locat_y(void);//回傳目前y軸的相對位置
	int* return_all_locat(void);//回傳所有目前全部等級的位置資訊。回傳的為一個陣列，0~2為等級0~2的x軸位置，3~5為等級0~2的y軸位置
	const Data_type return_fliter_setting(void);//回傳目前篩選類別

	int return_lotId(void);
	pair<int, int> return_locat_xy(int, int);

	string currentInfoList(void);
	map<int, int> return_defect_count(void);

	/*取得指定區塊限定die type的統計資料*/
	int get_statistics_data(const int& x, const int& y, const int& target);//return the number of specific type at corresponding location at level 0
	int get_statistics_data(const int& x0, const int& y0, const int& x1, const int& y1, const int& target);//return the number of specific type at corresponding location at level 1
	int get_statistics_data(const int& x0, const int& y0, const int& x1, const int& y1, const int& x2, const int& y2, const int& target);//return the number of specific type at corresponding location at level 2
	
	/*繪製圖例*/
	cv::Mat* lengend_paint(void);

	/*建構子，初始化陣列*/
	DataControlUnit()
	{
		index = new data_unit ** [10000];
		for (int i = 0; i < 10000; i++)
		{
			index[i] = new data_unit * [10000]();
		}
	}
};


