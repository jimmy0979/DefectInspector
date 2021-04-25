/*******************************************************************************
* DieMapper.h
 *******************************************************************************/

#pragma once
#include <string>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

 // Do Not use namesapce cv in .h file
 // or it will cause ACCESS_MASK problem (conflict with windows.h)
 // link : https://blog.csdn.net/xu20082100226/article/details/45482387


class DieMapper
{
private:
	// PROPERTIES
	int regionLength = 30;
	int ROWS = 16, COLS = 8;
	std::string windowName = "DieMapper";
	cv::Mat Canvas;

	// TRIGGER 

	// WARNING LEVEL
	// return different color depends on the #defect
	cv::Scalar WarningLevel(int defectNum);

	// DISPLAY
	// draw the divider line on the Canvas to help regonize the regions
	void dividerLine();

public:
	// constructor with default argument
	DieMapper(int rows = 16, int cols = 8, cv::Scalar initColor = cv::Scalar(0, 252, 124));

	// UPDATE
	// draw the selected region by its WarningLevel(depends on #defects)
	void paint(int region, int defectNum);

	// DISPLAY
	// display the Canvas by primitive imshow() function
	void show();
	// return the Mat of Canvas
	cv::Mat returnMat();
};

