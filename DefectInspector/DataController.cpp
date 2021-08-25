#include "DataController.h"
#define Data_type_offset 254 //用來位移篩選條件的值至defect type的值
/*
System::String DataControlUnit::return_jpgname(const int& level, const vector<__int64>& location, const vector<string>& filter)
{

}
*/
void DataControlUnit::create_floder()
{
	System::String^ path = "img_data";
	System::IO::Directory::CreateDirectory(path);
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			path = System::String::Format("img_data/{0}{1}", i, j);
			System::IO::Directory::CreateDirectory(path);
			for (int u = 0; u < 5; u++)
			{
				for (int h = 0; h < 5; h++)
				{
					path = System::String::Format("img_data/{0}{1}{2}{3}", i, j, u, h);
					System::IO::Directory::CreateDirectory(path);
				}
			}
		}
	}
}

void DataControlUnit::put_data(const int& x, const int& y,const int &defect, const string& bincode)
{
	if (index[y][x] == nullptr)
	{
		index[y][x] = new data_unit(defect, bincode);
	}
	else
	{
		delete_statistics(x, y);//減少相對應的統計個數
		index[y][x]->bin_code = bincode;
		index[y][x]->defect_type = defect;
	}
}
void DataControlUnit::put_data(const int& x, const int& y, const int& defect)
{
	if (index[y][x] == nullptr)
	{
		count_level_0[(y / 1000) * 10 + (x / 1000)]++;
		count_level_1[(y / 100) * 100 + (x / 100)]++;
		count_level_2[(y / 10) * 1000 + (x / 10)]++;
		index[y][x] = new data_unit(defect);
		if (index[y][x]->operator==(this->register_data) && ((y/100)*100+(x/100)) == ((this->last_locate_y/100)*100+(this->last_locate_x/100))){//檢查種類是否相同和區域是否相同
			this->counts++;
		}
		else {
			this->insert_statistics(this->last_locate_x, this->last_locate_y, &this->register_data, this->counts);
			this->last_locate_x = x;
			this->last_locate_y = y;
			this->register_data = *index[y][x];
			this->counts = 1;
		}
	}
	else//更新資料
	{
		delete_statistics(x, y);//減少相對應的統計個數
		index[y][x]->defect_type = defect;
		this->insert_statistics(x, y, index[y][x]);
	}
}

vector<paint_unit> DataControlUnit::pull_data(const bool& for_roi)
{
	vector<paint_unit> output;
	int start_x = 0,start_y = 0; 
	double percent;
	int block_size = 0;//ROI檢查區塊邊的大小 or Map含有的晶粒個數
	if (for_roi)
	{
		switch (this->level) {
		case 2:
			start_x = level_0_x * 1000 + level_1_x * 100 + level_2_x * 10;
			start_y = level_0_y * 1000 + level_1_y * 100 + level_2_y * 10;
			block_size = 10;
			break;
		case 1:
			start_x = level_0_x * 1000 + level_1_x * 100;
			start_y = level_0_y * 1000 + level_1_y * 100;
			block_size = 100;
			break;
		case 0:
			start_x = level_0_x * 1000;
			start_y = level_0_y * 1000;
			block_size = 1000;
		}
		for (int i = 0; i < block_size; i++){
			for (int j = 0; j < block_size; j++){
				switch (this->filter_variable) {
				case Data_type::Alldefect:
					if (index[start_y + i][start_x + j] != nullptr) {
						output.push_back(paint_unit(j, i, decide_color_roi(index[start_y + i][start_x + j])));
					}
					break;
				case Data_type::NormalDies:
					if (index[start_y + i][start_x + j] == nullptr) {
						output.push_back(paint_unit(j, i, decide_color_roi(index[start_y + i][start_x + j])));
					}
				}
			}
		}
	}
	else
	{
		int trans_locat = 0;//轉換過的起始位置
		int locat_offset = 0;//每個low之間的位移量
		int* curr_count_array = nullptr;
		switch (this->level) {
		case 2:
			trans_locat = level_0_y * 100000 + level_0_x * 100 + level_1_y * 10000 + level_1_x * 10;
			curr_count_array = count_level_2;
			block_size = 100;
			locat_offset = 1000;
			break;
		case 1:
			trans_locat = level_0_y * 1000 + level_0_x * 10;
			curr_count_array = count_level_1;
			block_size = 10000;
			locat_offset = 100;
			break;
		case 0:
			trans_locat = 0;
			curr_count_array = count_level_0;
			block_size = 1000000;
			locat_offset = 10;
		}
		if (curr_count_array != nullptr) {//避免取到nullptr
			for (int i = 0; i < 10; i++) {
				for (int j = 0; j < 10; j++) {
					switch (this->filter_variable) {
					case Data_type::Alldefect:
						percent = (double)(curr_count_array[trans_locat + i * locat_offset + j]) / block_size;
						break;
					case Data_type::NormalDies:
						percent = 1 - (double)(curr_count_array[trans_locat + i * locat_offset + j]) / block_size;
					}
					output.push_back(paint_unit(j, i, this->decide_color_map(percent)));
				}
			}
		}
	}
	return output;
}

bool DataControlUnit::update_data(const int& x, const int& y) {
	if (index[y][x] == nullptr)
		return false;

	count_level_0[(y / 1000) * 10 + (x / 1000)]--;
	count_level_1[(y / 100) * 100 + (x / 100)]--;
	count_level_2[(y / 10) * 1000 + (x / 10)]--;

	this->delete_statistics(x, y);
	free(index[y][x]);
	index[y][x] = nullptr;

	return true;
}

bool DataControlUnit::change_level(const bool& enlarge)
{
	if (enlarge)
	{
		if (level != 2)
			level++;
		else
			return false;
		if (level == 1)
		{
			level_1_x = level_1_y = 0;
		}
		else
		{
			level_2_x = level_2_y = 0;
		}
		return true;
	}
	else
	{
		if (level != 0)
		{
			level--;
			return true;
		}
		else
			return false;
	}
}

bool DataControlUnit::change_block(const int& direction)
{
	switch (direction) {
	case 0://left
		switch (this->level){
		case 0:
			if (level_0_x == 0)
				return false;
			level_0_x--;
			return true;
		case 1:
			if (level_1_x == 0){
				if (level_0_x == 0)
					return false;
				map_change = true;
				level_1_x = 9;
				level_0_x--;
			}
			else
				level_1_x--;
			return true;
		case 2:
			if (level_2_x == 0){
				if (level_0_x == 0 && level_1_x == 0)
					return false;
				map_change = true;
				level_2_x = 9;
				if (level_1_x == 0){
					level_1_x = 9;
					level_0_x--;
				}
				else
					level_1_x--;
				return true;
			}
			else
				level_2_x--;
			return true;
		}
	case 1://right
		switch (this->level) {
		case 0:
			if (level_0_x == 9)
				return false;
			level_0_x++;
			return true;
		case 1:
			if (level_1_x == 9){
				if (level_0_x == 9)
					return false;
				map_change = true;
				level_1_x = 0;
				level_0_x++;
			}
			else
				level_1_x++;
			return true;
		case 2:
			if (level_2_x == 9){
				if (level_0_x == 9 && level_1_x == 9)
					return false;
				map_change = true;
				level_2_x = 0;
				if (level_1_x == 9){
					level_1_x = 0;
					level_0_x++;
				}
				else
					level_1_x++;
				return true;
			}
			else
				level_2_x++;
			return true;
		}
	case 2://up
		switch (this->level){
		case 0:
			if (level_0_y == 0)
				return false;
			level_0_y--;
			return true;
		case 1:
			if (level_1_y == 0){
				if (level_0_y == 0)
					return false;
				map_change = true;
				level_1_y = 9;
				level_0_y--;
			}
			else
				level_1_y--;
			return true;
		case 2:
			if (level_2_y == 0){
				if (level_0_y == 0 && level_1_y == 0)
					return false;
				map_change = true;
				level_2_y = 9;
				if (level_1_y == 0){
					level_1_y = 9;
					level_0_y--;
				}
				else
					level_1_y--;
				return true;
			}
			else
				level_2_y--;
			return true;
		}
	case 3://down
		switch (this->level){
		case 0:
			if (level_0_y == 9)
				return false;
			level_0_y++;
			return true;
		case 1:
			if (level_1_y == 9){
				if (level_0_y == 9)
					return false;
				map_change = true;
				level_1_y = 0;
				level_0_y++;
			}
			else 
				level_1_y++;
			return true;
		case 2:
			if (level_2_y == 9){
				if (level_0_y == 9 && level_1_y == 9)
					return false;
				map_change = true;
				level_2_y = 0;
				if (level_1_y == 9){
					level_1_y = 0;
					level_0_y++;
				}
				else
					level_1_y++;
				return true;
			}
			else
				level_2_y++;
			return true;
		}
	}
	return false;
}

bool DataControlUnit::move_block(const int& moveX, const int& moveY)
{
	if (moveX == 0 && moveY == 0)
		return false;
	switch (this->level) {
	case 0:
		this->level_0_x += moveX;
		this->level_0_y += moveY;
		break;
	case 1:
		this->level_1_x += moveX;
		this->level_1_y += moveY;
		break;
	case 2:
		this->level_2_x += moveX;
		this->level_2_y += moveY;
	}
	return true;
}


bool DataControlUnit::change_filter(const int& type)//change the type of dies you can visible
{
	if ((int)this->filter_variable != type) {
		this->filter_variable = (Data_type)type;
		return true;
	}
	return false;
}

const Data_type DataControlUnit::return_fliter_setting(void)
{
	return this->filter_variable;
}

bool DataControlUnit::return_map_change(void)
{
	bool result;
	result = DataControlUnit::map_change;
	DataControlUnit::map_change = false;
	return result;
}
const int DataControlUnit::return_locat_x(void)
{
	if (level == 0)
	{
		return level_0_x;
	}
	else if (level == 1)
	{
		return level_1_x;
	}
	else
	{
		return level_2_x;
	}
}

const int DataControlUnit::return_locat_y(void)
{
	if (level == 0)
	{
		return level_0_y;
	}
	else if (level == 1)
	{
		return level_1_y;
	}
	else
	{
		return level_2_y;
	}
}
int* DataControlUnit::return_all_locat(void)
{
	int* data_array = new int[6];
	data_array[0] = this->level_0_x;
	data_array[1] = this->level_1_x;
	data_array[2] = this->level_2_x;
	data_array[3] = this->level_0_y;
	data_array[4] = this->level_1_y;
	data_array[5] = this->level_2_y;
	return data_array;
}
//color function
cv::Scalar DataControlUnit::decide_color_roi(const data_unit* input_data)
{
	switch (DataControlUnit::filter_variable)//the condition to decide which color show, show one color once
	{
	case Data_type::Alldefect:
		return cv::Scalar(0, 0, 255);
	case Data_type::NormalDies:
		return cv::Scalar(0, 255, 0);
	}
}
cv::Scalar DataControlUnit::decide_color_map(const double& percent)
{
	int v1, v2;//三個顏色的數值
	if (percent > 0.49) {//the value of percent more large means this type data ocuppy more area
		//Light 50%
		v1 = 0;
		v2 = 255;
	}
	else if (percent > 0.24){
		//65%
		v1 = 77;
		v2 = 255;
	}
	else if (percent > 0.124){
		//75%
		v1 = 128;
		v2 = 204;
	}
	else if (percent > 0.0624){
		//85%
		v1 = 179;
		v2 = 185;
	}
	else if (percent > 0.03124){
		//95%
		v1 = 230;
		v2 = 159;
	}
	else{
		//100%
		v1 = v2 = 255;
	}
	switch (DataControlUnit::filter_variable)//check exist defect or not
	{
	case Data_type::Alldefect:
		return cv::Scalar(v1, v1, v2);
	case Data_type::NormalDies:
		return cv::Scalar(v1, v2, v1);
	}
}

int DataControlUnit::return_lotId(void) {
	// return LOT_ID of current die
	// plus 1 since LOT_ID is 1-indexed 
	return (this->level_0_y * 100 + this->level_0_x) + 1;
}

pair<int, int> DataControlUnit::return_locat_xy(int curX, int curY) {
	// WARNING : let the imgROI can update Dies at any level, which can only update in level == 2
	int y = level_1_y * 100 + level_2_y * 10 + curY;
	int x = level_1_x * 100 + level_2_x * 10 + curX;
	return {x, y};
}

string DataControlUnit::currentInfoList() {
	return this->infoList;
}

map<int, int> DataControlUnit::return_defect_count() {
	int defectCnt = 0, tot = 0;
	switch (level) {
	case 0:
		tot = 1000000;
		defectCnt = count_level_0[level_0_y * 10 + level_0_x];
		break;
	case 1:
		tot = 10000;
		// defectCnt = count_level_1[(level_0_y / 100) * 100 + (level_0_x / 100)];
		defectCnt = count_level_1[level_0_y * 1000 + level_0_x * 10 + level_1_y * 100 + level_1_x];
		break;
	case 2:
		tot = 100;
		// defectCnt = count_level_2[(level_0_y / 10) * 1000 + (level_0_x / 10)];
		defectCnt = count_level_2[level_0_y * 100000 + level_0_x * 100 + level_1_y * 10000 + level_1_x * 10 + level_2_y * 1000 + level_2_x];
		break;
	default:
		defectCnt = 0;
	}

	map<int, int> res;
	res[0] = tot - defectCnt;
	res[1] = defectCnt;

	// real-time infoList
	stringstream ss;
	ss << "====================\n";
	ss << "Col 0 : " << this->level_0_x << ", Row 0 : " << this->level_0_y << endl;
	ss << "Col 1 : " << this->level_1_x << ", Row 1 : " << this->level_1_y << endl;
	ss << "Col 2 : " << this->level_2_x << ", Row 2 : " << this->level_2_y << endl;

	for(pair<int, int> i: res)
		ss << "DefectType: " << i.first << ", has defectNum: " << i.second << "\n";
	
	ss << "Yield Rate : " << (double)defectCnt / tot << "%\n";
	ss << "====================\n";
	infoList = ss.str();

	return res;
}

void DataControlUnit::insert_statistics(const int& x, const int& y, const data_unit* input_data) {/*未寫完差bincode*/
	if (input_data->defect_type != -1){//check whether exist defect
		map<int, statistics_node>::iterator itr = statistics_map.find(input_data->defect_type);
		if (itr == statistics_map.end()) {//chek whether key exist 
			statistics_map[input_data->defect_type] = statistics_node(x, y);//doesn't exist , add new node and insert data
		}
		else {
			itr->second.insert(x, y);//exist insert data directly
		}
	}
}

void DataControlUnit::insert_statistics(const int& x, const int& y, const data_unit* input_data, const int& num)
{
	if (num < 1)
		return;
	else {
		if (input_data->defect_type != -1) {//check whether exist defect
			map<int, statistics_node>::iterator itr = statistics_map.find(input_data->defect_type);
			if (itr == statistics_map.end()) {//chek whether key exist 
				statistics_map[input_data->defect_type] = statistics_node(x, y, num);//doesn't exist , add new node and insert data
			}
			else {
				itr->second.insert(x, y, num);//exist insert data directly
			}
		}
	}

}

void DataControlUnit::insert_done_reset()
{
	if (this->counts != 0) {
		this->insert_statistics(this->last_locate_x, this->last_locate_y, &this->register_data, this->counts);//insert data
		this->last_locate_x = this->last_locate_y = -1;//reset variable for next wafer insert
		this->counts = 0;
		this->register_data = data_unit(-1);
	}
}

bool DataControlUnit::delete_statistics(const int& x, const int& y) {/*未寫完差bincode*/
	if (this->index[y][x] != nullptr)
	{
		//確認是否有defect
		if (this->index[y][x]->defect_type != -1) {
			map<int, statistics_node>::iterator itr = statistics_map.find(index[y][x]->defect_type);//在map找到對映的value的iterator
			try {
				if(itr == statistics_map.end())
					throw gcnew System::Exception(gcnew System::String("The data in array isn't equal to map existing key, program is something wrong"));//陣列有此類別，map中沒有，數據不同步
				if(!itr->second.minus(x,y))
					throw gcnew System::Exception(gcnew System::String("The defect type exist at least one but map didn't count correctly, program is something wrong"));//陣列有此類別的晶粒，map裡的統計資料錯誤
			}
			catch (System::Exception^ e) {
				throw gcnew System::Exception(e->Message);//跳出錯誤代碼
			}
			this->index[y][x]->defect_type = -1;//將defect type重新初始化
			return true;
		}
	}
	return false;
}

void DataControlUnit::classify_BFS(const int range_x, const int range_y)
{
	bool vist[100][100] = { false };
	queue<pair<int, int>> q;
	data_unit temp;
	int curr_x = 0;
	int curr_y = 0;
	int curr_counts;
	int i = 0, j = 0;
	while (true){
		//設定起始點
		while(i < 100){
			while(j < 100){
				if (!vist[i][j]) {
					if (this->index[i + range_y * 100][j + range_x * 100] != nullptr) {
						temp = *this->index[i + range_y * 100][j + range_x * 100];
						//BFS
						curr_counts = 0;
						q.push(pair<int, int>(i, j));
						while(!q.empty()){
							if (q.front().first > 0 && q.front().first < 100 && q.front().second > 0 && q.front().second < 100) {
								if (!vist[q.front().first][q.front().second]) {
									if (*this->index[q.front().first + range_y * 100][q.front().second + range_x * 100] == temp) {
										++curr_counts;
										vist[q.front().first][q.front().second] = true;
										q.push(pair<int, int>(q.front().first + 1, q.front().second));
										q.push(pair<int, int>(q.front().first - 1, q.front().second));
										q.push(pair<int, int>(q.front().first, q.front().second + 1));
										q.push(pair<int, int>(q.front().first, q.front().second - 1));
									}
								}
							}
							q.pop();
						}
						insert_statistics(range_x * 100,range_y * 100,&temp,curr_counts);
					}
					else {
						vist[i][j] = true;
					}
				}
				j++;
			}
			j = 0;
			++i;
		}
	}
}

void DataControlUnit::Statistics_node::insert(const int& x, const int& y){
	node_count_level_0[(y / 1000) * 10 + (x / 1000)]++;
	node_count_level_1[(y / 100) * 100 + (x / 100)]++;
}

void DataControlUnit::Statistics_node::insert(const int& x, const int& y, const int& num)
{
	if (num < 1)
		return;
	node_count_level_0[(y / 1000) * 10 + (x / 1000)] += num;
	node_count_level_1[(y / 100) * 100 + (x / 100)] += num;
}

bool DataControlUnit::Statistics_node::minus(const int& x, const int& y)
{
	//檢查是否此區域之前是否真的有東西
	if (node_count_level_1[(y/100)*100+(x/100)]>0) {
		node_count_level_0[(y / 1000) * 10 + (x / 1000)]--;
		node_count_level_1[(y / 100) * 100 + (x / 100)]--;
		return true;
	}
	return false;//減少不存在的東西，減少失敗
}

int DataControlUnit::Statistics_node::search_statistics(const int& x, const int& y){
	return node_count_level_0[y * 10 + x];
}

int DataControlUnit::Statistics_node::search_statistics(const int& x0, const int& y0, const int& x1, const int& y1){
	return node_count_level_1[y0 * 1000 + x0 * 10 + y1 * 100 + x1];
}

int DataControlUnit::get_statistics_data(const int& x, const int& y, const int& target)
{
	map<int, statistics_node>::iterator itr = statistics_map.find(target);
	if (itr != statistics_map.end())//chek whether key exist 
	{
		return itr->second.search_statistics(x, y);
	}
	return 0;
}

int DataControlUnit::get_statistics_data(const int& x0, const int& y0, const int& x1, const int& y1, const int& target)
{
	map<int, statistics_node>::iterator itr = statistics_map.find(target);
	if (itr != statistics_map.end())//chek whether key exist 
	{
		return itr->second.search_statistics(x0, y0, x1, y1);
	}
	return 0;
}

int DataControlUnit::get_statistics_data(const int& x0, const int& y0, const int& x1, const int& y1, const int& x2, const int& y2, const int& target)
{
	int result = 0;
	int start_x = x0 * 1000 + x1 * 100 + x2 * 10;
	int start_y = y0 * 1000 + y1 * 100 + y2 * 10;
	for (int i = 0; i < 10; i++)//using Sequential Search
	{
		for (int j = 0; j < 10; j++)
		{
			if (index[start_y + i][start_x + j] == nullptr)
				continue;
			if (index[start_y + i][start_x + j]->defect_type == (253 + (int)this->filter_variable))
				result++;
		}
	}
	return result;
}

cv::Mat* DataControlUnit::lengend_paint(void)
{
	int v1,i = 0;
	cv::Mat* legend = new cv::Mat(cv::Size(100,180), CV_8UC3, cv::Scalar(255, 255, 255));
	while (i < 5){
		switch (i) {
		case 0:
			v1 = 0;
			break;
		case 1:
			v1 = 77;
			break;
		case 2:
			v1 = 128;
			break;
		case 3:
			v1 = 179;
			break;
		case 4:
			v1 = 230;
		}
		switch (this->filter_variable) {
		case Data_type::Alldefect:
			cv::rectangle(*legend, cv::Rect(0, 30 * i, 100, 30), cv::Scalar(v1, v1, 255), -1);
			break;
		case Data_type::NormalDies:
			cv::rectangle(*legend, cv::Rect(0, 30 * i, 100, 30), cv::Scalar(v1, 255, v1), -1);
		}
		i++;
	}
	return legend;
}

bool DataControlUnit::Data_Unit::operator==(const Data_Unit& comp) const
{
	if (this->defect_type != -1)
		return this->defect_type == comp.defect_type;
	return false;
}

void DataControlUnit::Data_Unit::operator=(const Data_Unit& b)
{
	this->defect_type = b.defect_type;
	this->bin_code = b.bin_code;
}
