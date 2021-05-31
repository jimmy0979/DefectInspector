#pragma once
#include "DataController.h"
#include "Map.h"
#include "ROI.h"
#include <ctime>
#include <opencv2/highgui/highgui_c.h>//for define event
#include <random>//for random
using namespace std;

Map* die_map = new Map;
ROI* roi = new ROI(900, 900);
DataControlUnit* data_controller = new DataControlUnit;
void on_mouse(int EVENT, int x, int y, int flag, void* ustc);
clock_t start, end_time;

int main(void)
{
	double defeat_rate = 0.1;
	cout << "input defeat rate:";
	cin >> defeat_rate;
	default_random_engine generator;
	bernoulli_distribution distribution(defeat_rate);
	int key_value=-1;
	cv::Mat control = cv::imread("controlPanel.jpg");
	start = clock();
	for (int i = 0; i < 10000; i++)
	{
		for (int j = 0; j < 10000; j++)
		{
			if(distribution(generator))
				data_controller->put_data(j,i,205);
		}
	}
	end_time = clock();
	cout << "push_back():" << ((double)(end_time - start)) / CLOCKS_PER_SEC << "s\n";
	start = clock();
	cv::imshow("controlPanel",control);
	cv::imshow("ROI", roi->show(data_controller->pull_data(true), data_controller->return_level()));
	cv::imshow("Map", die_map->show(data_controller->pull_data(false), data_controller->return_level(), data_controller->return_locat_x(), data_controller->return_locat_y()));
	end_time = clock();
	cout << "initial time:" << ((double)(end_time - start)) / CLOCKS_PER_SEC << "s\n";
	cv::setMouseCallback("controlPanel", on_mouse, 0);
	/*
	while (true)
	{
		key_value =  cv::waitKey(10);
		if (key_value == 38)
		{
			if (data_controller->change_block(2))
			{
				cv::imshow("ROI", roi->show(data_controller->pull_data(true), data_controller->return_level()));
				cv::imshow("Map", die_map->relocate(data_controller->return_locat_x(), data_controller->return_locat_y(), data_controller->return_level()));
			}
			if (data_controller->return_map_change())
			{
				cv::imshow("Map", die_map->show(data_controller->pull_data(false), data_controller->return_level(), data_controller->return_locat_x(), data_controller->return_locat_y()));
			}
		}
		else if (key_value == 40)
		{
			if (data_controller->change_block(3))
			{
				cv::imshow("ROI", roi->show(data_controller->pull_data(true), data_controller->return_level()));
				cv::imshow("Map", die_map->relocate(data_controller->return_locat_x(), data_controller->return_locat_y(), data_controller->return_level()));
			}
			if (data_controller->return_map_change())
			{
				cv::imshow("Map", die_map->show(data_controller->pull_data(false), data_controller->return_level(), data_controller->return_locat_x(), data_controller->return_locat_y()));
			}
		}
		else if (key_value == 37)
		{
			if (data_controller->change_block(0))
			{
				cv::imshow("ROI", roi->show(data_controller->pull_data(true), data_controller->return_level()));
				cv::imshow("Map", die_map->relocate(data_controller->return_locat_x(), data_controller->return_locat_y(), data_controller->return_level()));
			}
			if (data_controller->return_map_change())
			{
				cv::imshow("Map", die_map->show(data_controller->pull_data(false), data_controller->return_level(), data_controller->return_locat_x(), data_controller->return_locat_y()));
			}
		}
		else if (key_value == 39)
		{
			if (data_controller->change_block(1))
			{
				cv::imshow("ROI", roi->show(data_controller->pull_data(true), data_controller->return_level()));
				cv::imshow("Map", die_map->relocate(data_controller->return_locat_x(), data_controller->return_locat_y(), data_controller->return_level()));
			}
			if (data_controller->return_map_change())
			{
				cv::imshow("Map", die_map->show(data_controller->pull_data(false), data_controller->return_level(), data_controller->return_locat_x(), data_controller->return_locat_y()));
			}
		}
		else if (key_value == 107)
		{
			if (data_controller->change_level(true))
			{
				cv::imshow("Map", die_map->show(data_controller->pull_data(false), data_controller->return_level(), data_controller->return_locat_x(), data_controller->return_locat_y()));
				cv::imshow("ROI", roi->show(data_controller->pull_data(true), data_controller->return_level()));
			}
		}
		else if (key_value == 109)
		{
			if (data_controller->change_level(false))
			{
				cv::imshow("Map", die_map->show(data_controller->pull_data(false), data_controller->return_level(), data_controller->return_locat_x(), data_controller->return_locat_y()));
				cv::imshow("ROI", roi->show(data_controller->pull_data(true), data_controller->return_level()));
			}
		}
		else if (key_value == 27)
		{
			break;
		}
	}
	*/
	//cv::resize(img, img, cv::Size(900, 900), 0, 0, cv::INTER_LINEAR);
	//cv::imshow("test2", img);
	cv::waitKey();
	return 0;
}

void on_mouse(int EVENT, int x, int y, int flags, void* ustc)
{
	static int pre_block = -1;
	static int cur_block = -1;
	static int key = -1;
	if (EVENT == CV_EVENT_LBUTTONDOWN)
	{
		if (y / 100 == 0)
		{
			if (x / 100 == 0)
			{
				pre_block = 0;
			}
			else if (x / 100 == 1)
			{
				pre_block = 1;
			}
			else if (x / 100 == 2)
			{
				pre_block = 2;
			}

		}
		else if (y / 100 == 1)
		{
			if (x / 100 == 0)
			{
				pre_block = 3;
			}
			else if (x / 100 == 1)
			{
				pre_block = 4;
			}
			else if (x / 100 == 2)
			{
				pre_block = 5;
			}
		}
		else
			pre_block = -1;
	}
	else if (EVENT == CV_EVENT_LBUTTONUP)
	{
		if (y / 100 == 0)
		{
			if (x / 100 == 0)
			{
				cur_block = 0;
			}
			else if (x / 100 == 1)
			{
				cur_block = 1;
			}
			else if (x / 100 == 2)
			{
				cur_block = 2;
			}

		}
		else if (y / 100 == 1)
		{
			if (x / 100 == 0)
			{
				cur_block = 3;
			}
			else if (x / 100 == 1)
			{
				cur_block = 4;
			}
			else if (x / 100 == 2)
			{
				cur_block = 5;
			}
		}
		else
			cur_block = -1;
	}
	if (pre_block == cur_block && pre_block != -1)
	{
		start = clock();
		cout << pre_block << endl;
		if (cur_block == 1)
		{
			if (data_controller->change_block(2))
			{
				cv::imshow("ROI", roi->show(data_controller->pull_data(true), data_controller->return_level()));
				cv::imshow("Map", die_map->relocate(data_controller->return_locat_x(), data_controller->return_locat_y(), data_controller->return_level()));
			}
			if (data_controller->return_map_change())
			{
				cv::imshow("Map", die_map->show(data_controller->pull_data(false), data_controller->return_level(), data_controller->return_locat_x(), data_controller->return_locat_y()));
			}
		}
		else if (cur_block == 4)
		{
			if (data_controller->change_block(3))
			{
				cv::imshow("ROI", roi->show(data_controller->pull_data(true), data_controller->return_level()));
				cv::imshow("Map", die_map->relocate(data_controller->return_locat_x(), data_controller->return_locat_y(), data_controller->return_level()));
			}
			if (data_controller->return_map_change())
			{
				cv::imshow("Map", die_map->show(data_controller->pull_data(false), data_controller->return_level(), data_controller->return_locat_x(), data_controller->return_locat_y()));
			}
		}
		else if (cur_block == 0)
		{
			if (data_controller->change_block(0))
			{
				cv::imshow("ROI", roi->show(data_controller->pull_data(true), data_controller->return_level()));
				cv::imshow("Map", die_map->relocate(data_controller->return_locat_x(), data_controller->return_locat_y(), data_controller->return_level()));
			}
			if (data_controller->return_map_change())
			{
				cv::imshow("Map", die_map->show(data_controller->pull_data(false), data_controller->return_level(), data_controller->return_locat_x(), data_controller->return_locat_y()));
			}
		}
		else if (cur_block == 3)
		{
			if (data_controller->change_block(1))
			{
				cv::imshow("ROI", roi->show(data_controller->pull_data(true), data_controller->return_level()));
				cv::imshow("Map", die_map->relocate(data_controller->return_locat_x(), data_controller->return_locat_y(), data_controller->return_level()));
			}
			if (data_controller->return_map_change())
			{
				cv::imshow("Map", die_map->show(data_controller->pull_data(false), data_controller->return_level(), data_controller->return_locat_x(), data_controller->return_locat_y()));
			}
		}
		else if (cur_block == 2)
		{
			if (data_controller->change_level(true))
			{
				cv::imshow("Map", die_map->show(data_controller->pull_data(false), data_controller->return_level(), data_controller->return_locat_x(), data_controller->return_locat_y()));
				cv::imshow("ROI", roi->show(data_controller->pull_data(true), data_controller->return_level()));
			}
		}
		else if (cur_block == 5)
		{
			if (data_controller->change_level(false))
			{
				cv::imshow("Map", die_map->show(data_controller->pull_data(false), data_controller->return_level(), data_controller->return_locat_x(), data_controller->return_locat_y()));
				cv::imshow("ROI", roi->show(data_controller->pull_data(true), data_controller->return_level()));
			}
		}
		pre_block = cur_block = -1;
		end_time = clock();
		cout << "refesh time:" << ((double)(end_time - start)) / CLOCKS_PER_SEC << "s\n";
	}
}
