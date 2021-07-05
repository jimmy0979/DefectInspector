#include "DataController.h"
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

void DataControlUnit::put_data(const int& x, const int& y,const int &defeat, const string& bincode)
{
	if (index[y][x] == nullptr)
	{
		index[y][x] = new data_unit(x, y, defeat, bincode);
	}
	else
	{
		index[y][x]->bin_code = bincode;
		index[y][x]->defeat_type = defeat;
	}
	insert_statistics(x, y, index[y][x]);
}
void DataControlUnit::put_data(const int& x, const int& y, const int& defeat)
{
	if (index[y][x] == nullptr)
	{
		index[y][x] = new data_unit(x, y, defeat);
	}
	else
	{
		index[y][x]->defeat_type = defeat;
	}
	insert_statistics(x, y, index[y][x]);
}

vector<paint_unit> DataControlUnit::pull_data(const bool& for_roi)
{
	vector<paint_unit> output;
	data_unit* temp;
	int start_x ,start_y; 
	double percent;
	cv::Scalar c;
	if (for_roi)
	{
		if (level == 2)
		{
			start_x = level_0_x * 1000 + level_1_x * 100 + level_2_x * 10;
			start_y = level_0_y * 1000 + level_1_y * 100 + level_2_y * 10;
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					if (index[start_y+i][start_x+j] != nullptr)
					{
						output.push_back(paint_unit(j, i, decide_color_roi(index[start_y + i][start_x + j])));
					}
				}
			}
		}
		else if (level == 1)
		{
			start_x = level_0_x * 1000 + level_1_x * 100;
			start_y = level_0_y * 1000 + level_1_y * 100;
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < 100; j++)
				{
					if (index[start_y + i][start_x + j] != nullptr)
					{
						output.push_back(paint_unit(j, i, decide_color_roi(index[start_y + i][start_x + j])));
					}
				}
			}
		}
		else
		{
			start_x = level_0_x * 1000;
			start_y = level_0_y * 1000;
			for (int i = 0; i < 1000; i++)
			{
				for (int j = 0; j < 1000; j++)
				{
					if (index[start_y + i][start_x + j] != nullptr)
					{
						output.push_back(paint_unit(j, i, decide_color_roi(index[start_y + i][start_x + j])));
					}
				}
			}
		}
	}
	else
	{
		if (level == 2)
		{
			for (int i = 0;  i <  10; i++)
			{
				for (int j = 0;  j <  10; j++)
				{
					percent = (double)(get_statistics_data(level_0_x, level_0_y, level_1_x, level_1_y, j, i, "256")) / 100;//這裡為假定defeat type為256
					output.push_back(paint_unit(j, i, DataControlUnit::decide_color_map(percent)));
				}
			}
		}
		else if (level == 1)
		{
			for (int i = 0;  i <  10; i++)
			{
				for (int j = 0;  j <  10; j++)
				{
					percent = (double)(get_statistics_data(level_0_x, level_0_y, j, i, "256")) / 10000;
					output.push_back(paint_unit(j, i, DataControlUnit::decide_color_map(percent)));
				}
			}
		}
		else
		{
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					percent = (double)(get_statistics_data(j, i, "256")) / 1000000;
					output.push_back(paint_unit(j, i, DataControlUnit::decide_color_map(percent)));
				}
			}
		}
	}
	return output;
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
	if (direction == 0)//left
	{
		if (level == 0)
		{
			if (level_0_x == 0)
				return false;
			level_0_x--;
			return true;
		}
		else if(level == 1)
		{
			if (level_1_x == 0)
			{
				if (level_0_x == 0)
					return false;
				map_change = true;
				level_1_x = 9;
				level_0_x--;
			}
			else
				level_1_x--;
			return true;
		}
		else if (level == 2)
		{
			if (level_2_x == 0)
			{
				if (level_0_x == 0 && level_1_x == 0)
					return false;
				map_change = true;
				level_2_x = 9;
				if (level_1_x == 0)
				{
					level_1_x = 9;
					level_0_x--;
				}
				else
					level_1_x--;
				return true;
			}
			else
			{
				level_2_x--;
				return true;
			}
		}
	}
	else if (direction == 1)//right
	{
		if (level == 0)
		{
			if (level_0_x == 9)
				return false;
			level_0_x++;
			return true;
		}
		else if (level == 1)
		{
			if (level_1_x == 9)
			{
				if (level_0_x == 9)
					return false;
				map_change = true;
				level_1_x = 0;
				level_0_x++;
			}
			else
				level_1_x++;
			return true;
		}
		else if (level == 2)
		{
			if (level_2_x == 9)
			{
				if (level_0_x == 9 && level_1_x == 9)
					return false;
				map_change = true;
				level_2_x = 0;
				if (level_1_x == 9)
				{
					level_1_x = 0;
					level_0_x++;
				}
				else
					level_1_x++;
				return true;
			}
			else
			{
				level_2_x++;
				return true;
			}
		}
	}
	else if (direction == 2)//up
	{
		if (level == 0)
		{
			if (level_0_y == 0)
				return false;
			level_0_y--;
			return true;
		}
		else if (level == 1)
		{
			if (level_1_y == 0)
			{
				if (level_0_y == 0)
					return false;
				map_change = true;
				level_1_y = 9;
				level_0_y--;
			}
			else
				level_1_y--;
			return true;
		}
		else if (level == 2)
		{
			if (level_2_y == 0)
			{
				if (level_0_y == 0 && level_1_y == 0)
					return false;
				map_change = true;
				level_2_y = 9;
				if (level_1_y == 0)
				{
					level_1_y = 9;
					level_0_y--;
				}
				else
					level_1_y--;
				return true;
			}
			else
			{
				level_2_y--;
				return true;
			}
		}
	}
	else if (direction == 3)//down
	{
		if (level == 0)
		{
			if (level_0_y == 9)
				return false;
			level_0_y++;
			return true;
		}
		else if (level == 1)
		{
			if (level_1_y == 9)
			{
				if (level_0_y == 9)
					return false;
				map_change = true;
				level_1_y = 0;
				level_0_y++;
		}
		else
			level_1_y++;
		return true;
		}
		else if (level == 2)
		{
			if (level_2_y == 9)
			{
				if (level_0_y == 9 && level_1_y == 9)
					return false;
				map_change = true;
				level_2_y = 0;
				if (level_1_y == 9)
				{
					level_1_y = 0;
					level_0_y++;
				}
				else
					level_1_y++;
				return true;
			}
			else
			{
				level_2_y++;
				return true;
			}
		}
	}
	return false;
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
//color function
cv::Scalar DataControlUnit::decide_color_roi(const data_unit* input_data)
{
	switch (DataControlUnit::filter_variable)//篩選條件，一次只會顯示一種
	{
	case 0://defeat
		return cv::Scalar(0, 0, 255);

	}
}
cv::Scalar DataControlUnit::decide_color_map(const double& percent)
{
	switch (DataControlUnit::filter_variable)//篩選條件，一次只會顯示一種
	{
	case 0://defeat
		if (percent > 0.49)//level 6 level越大，表示佔的比例越多
		{
			return cv::Scalar(0, 0, 255);
		}
		else if (percent > 0.25)//level 5
		{
			return cv::Scalar(80, 80, 255);
		}
		else if (percent > 0.125)//level 4
		{
			return cv::Scalar(102, 102, 255);
		}
		else if (percent > 0.0625)//level 3
		{
			return cv::Scalar(204, 204, 255);
		}
		else if (percent > 0.03125)//level 2
		{
			return cv::Scalar(204, 255, 255);
		}
		else//level 1
		{
			return cv::Scalar(0, 255, 0);
		}
	}
}

//about filter  新的
void DataControlUnit::insert_statistics(const int& x, const int& y, const data_unit* input_data)
{
	if (input_data->defeat_type != 0)//檢查是否有defeat
	{
		map<string, statistics_node>::iterator itr = statistics_map.find(to_string(input_data->defeat_type));
		if (itr == statistics_map.end())//檢查是否已存在此分類
		{
			statistics_map[to_string(input_data->defeat_type)] = statistics_node(x, y);//不存在，新增node
		}
		else
		{
			itr->second.insert(x, y);//已存在
		}
	}
	if (!input_data->bin_code.empty())//檢查是否有bin code
	{
		map<string, statistics_node>::iterator itr = statistics_map.find(input_data->bin_code);
		if (itr == statistics_map.end())
		{
			statistics_map[to_string(input_data->defeat_type)] = statistics_node(x, y);
		}
		else
		{
			itr->second.insert(x, y);
		}
	}
}

int DataControlUnit::get_statistics_data(const int& x, const int& y, const string& target)
{
	map<string, statistics_node>::iterator itr = statistics_map.find(target);
	int result;
	if (itr != statistics_map.end())//檢查是否已存在此分類
	{
		result = itr->second.search_statistics(x, y);
		return result;
	}
	return 0;
	return result;
}

int DataControlUnit::get_statistics_data(const int& x0, const int& y0, const int& x1, const int& y1, const string& target)
{
	map<string, statistics_node>::iterator itr = statistics_map.find(target);
	int result;
	if (itr != statistics_map.end())//檢查是否已存在此分類
	{
		result = itr->second.search_statistics(x0, y0, x1, y1);
		return result;
	}
	return 0;
}

int DataControlUnit::get_statistics_data(const int& x0, const int& y0, const int& x1, const int& y1, const int& x2, const int& y2, const string& target)
{
	int result = 0;
	int start_x = x0 * 1000 + x1 * 100 + x2 * 10;
	int start_y = y0 * 1000 + y1 * 100 + y2 * 10;
	for (int i = 0; i < 10; i++)//直接用循序搜尋法(Sequential Search)
	{
		for (int j = 0; j < 10; j++)
		{
			if (index[start_y + i][start_x + j] == nullptr)
				continue;
			if (index[start_y + i][start_x + j]->bin_code == target || to_string(index[start_y + i][start_x + j]->defeat_type) == target)
				result++;
		}
	}
	return result;
}
void DataControlUnit::Statistics_node::insert(const int& x, const int& y)
{
	node_count_level_0[(y / 1000) * 10 + (x / 1000)]++;
	node_count_level_1[(y / 100) * 100 + (x / 100)]++;
}

int DataControlUnit::Statistics_node::search_statistics(const int& x, const int& y)
{
	return node_count_level_0[y * 10 + x];
}

int DataControlUnit::Statistics_node::search_statistics(const int& x0, const int& y0, const int& x1, const int& y1)
{
	return node_count_level_1[y0 * 1000 + x0 * 10 + y1 * 100 + x1];
}