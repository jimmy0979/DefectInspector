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
	if (index[y * 10000 + x] == nullptr)
	{
		index[y * 10000 + x] = new data_unit(x, y, defeat, bincode);
	}
	else
	{
		index[y * 10000 + x]->bin_code = bincode;
		index[y * 10000 + x]->defeat_type = defeat;
	}
}
void DataControlUnit::put_data(const int& x, const int& y, const int& defeat)
{
	count_level_0[(y / 1000) * 10 + (x / 1000)]++;
	count_level_1[(y / 100) * 100 + (x / 100)]++;
	count_level_2[(y / 10) * 1000 + (x / 10)]++;
	if (index[y * 10000 + x] == nullptr)
	{
		index[y * 10000 + x] = new data_unit(x, y, defeat);
	}
	else
	{
		index[y * 10000 + x]->defeat_type = defeat;
	}
}

vector<paint_unit> DataControlUnit::pull_data(const bool& for_roi)
{
	vector<paint_unit> output;
	data_unit* temp;
	int start_x ,start_y; 
	if (for_roi)
	{
		if (level == 2)
		{
			start_x = level_0_x * 1000 + level_1_x * 100 + level_2_x;
			start_y = level_0_y * 1000 + level_1_y * 100 + level_2_y;
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					if (index[(start_y + i) * 10000 + start_x + j] != nullptr)
					{
						output.push_back(paint_unit(j, i, cv::Scalar(0, 255, 0)));
					}
				}
			}
		}
		else if (level == 1)
		{
			start_x = level_0_x * 1000 + level_1_x;
			start_y = level_0_y * 1000 + level_1_y;
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < 100; j++)
				{
					if (index[(start_y + i) * 10000 + start_x + j] != nullptr)
					{
						output.push_back(paint_unit(j, i, cv::Scalar(0, 255, 0)));
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
					if (index[(start_y + i) * 10000 + start_x + j] != nullptr)
					{
						output.push_back(paint_unit(j, i, cv::Scalar(0, 255, 0)));
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
					output.push_back(paint_unit(j, i, cv::Scalar((double)(count_level_1[level_0_y * 100000 + level_0_x * 100 + level_1_y * 10000 + level_1_x * 10 + i * 1000 + j]) / 100 * 255, (double)(100 - count_level_1[level_0_y * 100000 + level_0_x * 100 + level_1_y * 10000 + level_1_x * 10 + i * 1000 + j]) / 100 * 255, 0)));
				}
			}
		}
		else if (level == 1)
		{
			for (int i = 0;  i <  10; i++)
			{
				for (int j = 0;  j <  10; j++)
				{
					output.push_back(paint_unit(j, i, cv::Scalar((double)(count_level_1[level_0_y * 1000 + level_0_x * 10 + i * 100 + j]) / 10000 * 255, (double)(10000 - count_level_1[level_0_y * 1000 + level_0_x * 10 + i * 100 + j]) / 10000 * 255, 0)));
				}
			}
		}
		else
		{
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < 10; j++)
				{
					output.push_back(paint_unit(j, i, cv::Scalar((double)(count_level_0[i * 10 + j]) / 10000 * 255, (double)(10000 - count_level_0[i * 10 + j]) / 10000 * 255, 0)));
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
				level_1_x == 9;
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
				level_1_x == 0;
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
				level_1_y == 9;
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
				level_1_y == 0;
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
		return level_0_x;
	else if (level == 1)
		return level_1_x;
	else
		return level_2_x;
}
const int DataControlUnit::return_locat_y(void)
{
	if (level == 0)
		return level_0_y;
	else if (level == 1)
		return level_1_y;
	else
		return level_2_y;
}