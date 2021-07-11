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
}
void DataControlUnit::put_data(const int& x, const int& y, const int& defeat)
{
	if (index[y][x] == nullptr)
	{
		count_level_0[(y / 1000) * 10 + (x / 1000)]++;
		count_level_1[(y / 100) * 100 + (x / 100)]++;
		count_level_2[(y / 10) * 1000 + (x / 10)]++;

		index[y][x] = new data_unit(x, y, defeat);
	}
	else
	{
		index[y][x]->defeat_type = defeat;
	}
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
					percent = (double)(count_level_2[level_0_y * 100000 + level_0_x * 100 + level_1_y * 10000 + level_1_x * 10 + i * 1000 + j]) / 100;
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
					percent = (double)(count_level_1[level_0_y * 1000 + level_0_x * 10 + i * 100 + j]) / 10000;
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
					percent = (double)(count_level_0[i * 10 + j]) / 1000000;
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
	switch (DataControlUnit::filter_variable)//the condition to decide which color show, show one color once
	{
	case 0://defeat
		return cv::Scalar(0, 0, 255);

	}
}
cv::Scalar DataControlUnit::decide_color_map(const double& percent)
{
	switch (DataControlUnit::filter_variable)//check exist defeat or not
	{
	case 0://defeat
		if (percent > 0.49)//level 6 level the value of percent more large means this type data ocuppy more area
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

int DataControlUnit::return_lotId(void) {
	// return LOT_ID of current die
	// plus 1 since LOT_ID is 1-indexed 
	return (this->level_0_y * 100 + this->level_0_x) + 1;
}

pair<int, int> DataControlUnit::return_locat_xy(int curX, int curY) {
	// WARNING : let the imgROI can update Dies at any level, which can only update in level == 2
	int y = level_1_y * 100 + curY;
	int x = level_1_x * 100 + curX;
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
	ss << "Col 0 : " << this->level_0_x << ", Row 0 : " << this->level_0_y << endl;
	ss << "Col 1 : " << this->level_1_x << ", Row 1 : " << this->level_1_y << endl;
	ss << "Col 2 : " << this->level_2_x << ", Row 2 : " << this->level_2_y << endl;

	for(pair<int, int> i: res)
		ss << i.first << " -> " << i.second << "\n";
	infoList = ss.str();

	return res;
}