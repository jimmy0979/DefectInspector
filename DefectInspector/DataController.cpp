#include "DataController.h"

System::String DataControlUnit::return_jpgname(const int& level, const vector<__int64>& location, const vector<string>& filter)
{

}

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

void DataControlUnit::put_data(const __int64& x, const __int64& y,const __int64 &defeat, const string& bincode)
{
	data_unit* new_unit = new data_unit(x, y, defeat, bincode);
	if (index[(y / 10) * 1000 + (x / 10)] == nullptr)
	{
		index[(y / 10) * 1000 + (x / 10)] = new_unit;
		new_unit->pre = new_unit;
		new_unit->next = new_unit;
	}
	else
	{
		data_unit* temp = index[(y / 10) * 1000 + (x / 10)]->pre;
		temp->next = new_unit;
		index[(y / 10) * 1000 + (x / 10)]->pre = new_unit;
		new_unit->pre = temp;
		new_unit->next = index[(y / 10) * 1000 + (x / 10)];
	}
}
void DataControlUnit::put_data(const __int64& x, const __int64& y, const __int64& defeat)
{
	data_unit* new_unit = new data_unit(x, y, defeat);
	count_level_0[(y / 1000000) * 10 + (x / 1000000)]++;
	count_level_1[(y / 10000) * 100 + (x / 10000)]++;
	count_level_2[(y / 100) * 1000 + (x / 100)]++;
	if (index[(y / 10) * 1000 + (x / 10)] == nullptr)
	{
		index[(y / 10) * 1000 + (x / 10)] = new_unit;
		new_unit->pre = new_unit;
		new_unit->next = new_unit;
	}
	else
	{
		data_unit* temp = index[(y / 10) * 1000 + (x / 10)]->pre;
		temp->next = new_unit;
		index[(y / 10) * 1000 + (x / 10)]->pre = new_unit;
		new_unit->pre = temp;
		new_unit->next = index[(y / 10) * 1000 + (x / 10)];
	}
}

vector<paint_unit> DataControlUnit::pull_data(const bool& for_roi)
{
	vector<paint_unit> output;
	data_unit* temp;
	__int64 start_x = level_0_x * 1000 + level_1_x * 100 + level_2_x; 
	__int64 start_y = level_0_y * 1000 + level_1_y * 100 + level_2_y;
	if (for_roi)
	{
		if (level == 2)
		{
			temp = index[start_y * 1000 + start_x];
			if (temp == nullptr)
				return output;
			else
			{
				do
				{
					output.push_back(paint_unit(level_2_x, level_2_y, cv::Scalar(255, 0, 0)));
					temp = temp->next;
				} while (temp!=index[start_y*1000+start_x]);
			}
		}
		else if (level == 1)
		{
			for (__int64 i = 0;  i < 100; i++)
			{
				for (__int64 j = 0; start_x + j < start_x + 100; j++)
				{
					output.push_back(paint_unit(j, i, cv::Scalar((double)(count_level_2[level_0_y*100000 + level_0_x * 100 + level_1_y * 10000 + level_1_x * 10 + i * 1000 + j]) / 100 * 255, (double)(100 - count_level_2[level_0_y * 100000 + level_0_x * 100 + level_1_y * 10000 + level_1_x * 10 + i * 1000 + j]) / 100 * 255, 0)));
				}
			}
		}
		else
		{
			for (__int64 i = 0; i <  1000; i++)
			{
				for (__int64 j = 0; start_x + j < start_x + 1000; j++)
				{
					output.push_back(paint_unit(j, i, cv::Scalar((double)(count_level_1[level_0_y * 1000 + level_0_x * 10 + i * 100 + j]) / 10000*255, (double)(10000 - count_level_1[level_0_y * 1000 + level_0_x * 10 + i * 100 + j]) / 10000*255, 0)));
				}
			}
		}
	}
	else
	{
		if (level == 2)
		{
			for (__int64 i = 0;  i <  10; i++)
			{
				for (__int64 j = 0;  j <  10; j++)
				{
					output.push_back(paint_unit(j, i, cv::Scalar((double)(count_level_1[level_0_y * 100000 + level_0_x * 100 + level_1_y * 10000 + level_1_x * 10 + i * 1000 + j]) / 100 * 255, (double)(100 - count_level_1[level_0_y * 100000 + level_0_x * 100 + level_1_y * 10000 + level_1_x * 10 + i * 1000 + j]) / 100 * 255, 0)));
				}
			}
		}
		else if (level == 1)
		{
			for (__int64 i = 0;  i <  10; i++)
			{
				for (__int64 j = 0;  j <  10; j++)
				{
					output.push_back(paint_unit(j, i, cv::Scalar((double)(count_level_1[level_0_y * 1000 + level_0_x * 10 + i * 100 + j]) / 10000 * 255, (double)(10000 - count_level_1[level_0_y * 1000 + level_0_x * 10 + i * 100 + j]) / 10000 * 255, 0)));
				}
			}
		}
		else
		{
			for (__int64 i = 0; i < 10; i++)
			{
				for (__int64 j = 0; j < 10; j++)
				{
					output.push_back(paint_unit(j, i, cv::Scalar((double)(count_level_0[i * 10 + j]) / 10000 * 255, (double)(10000 - count_level_0[i * 10 + j]) / 10000 * 255, 0)));
				}
			}
		}
	}
	return output;
}
