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
	count_level_0[(y / 1000) * 10 + (x / 1000)]++;
	count_level_1[(y / 100) * 100 + (x / 100)]++;
	count_level_2[(y / 10) * 1000 + (x / 10)]++;
	if (index[y][x] == nullptr)
	{
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
						output.push_back(paint_unit(j, i, cv::Scalar(0, 0, 255)));
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
						output.push_back(paint_unit(j, i, cv::Scalar(0, 0, 255)));
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
						output.push_back(paint_unit(j, i, cv::Scalar(0, 0, 255)));
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
					if (percent > 0.49)
					{
						c = cv::Scalar(0, 0, 255);
					}
					else if (percent > 0.25)
					{
						c = cv::Scalar(80, 80, 255);
					}
					else if (percent > 0.125)
					{
						c = cv::Scalar(102, 102, 255);
					}
					else if (percent > 0.0625)
					{
						c = cv::Scalar(204, 204, 255);
					}
					else if (percent > 0.03125)
					{
						c = cv::Scalar(204, 255, 255);
					}
					else
					{
						c = cv::Scalar(0, 255, 0);
					}
					output.push_back(paint_unit(j, i, c));
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
					if (percent > 0.49)
					{
						c = cv::Scalar(0, 0, 255);
					}
					else if (percent > 0.25)
					{
						c = cv::Scalar(80, 80, 255);
					}
					else if (percent > 0.125)
					{
						c = cv::Scalar(102, 102, 255);
					}
					else if (percent > 0.0625)
					{
						c = cv::Scalar(204, 204, 255);
					}
					else if (percent > 0.03125)
					{
						c = cv::Scalar(204, 255, 255);
					}
					else
					{
						c = cv::Scalar(0, 255, 0);
					}
					output.push_back(paint_unit(j, i, c));
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
					if (percent > 0.49)
					{
						c = cv::Scalar(0, 0, 255);
					}
					else if (percent > 0.25)
					{
						c = cv::Scalar(80, 80, 255);
					}
					else if (percent > 0.125)
					{
						c = cv::Scalar(102,102 , 255);
					}
					else if (percent > 0.0625)
					{
						c = cv::Scalar(204, 204, 255);
					}
					else if (percent > 0.03125)
					{
						c = cv::Scalar(204, 255, 255);
					}
					else
					{
						c = cv::Scalar(0, 255, 0);
					}
					output.push_back(paint_unit(j, i, c));
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

	return res;
}