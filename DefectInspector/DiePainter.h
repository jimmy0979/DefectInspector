/*******************************************************************************
* DiePainter.h
 *******************************************************************************/

#pragma once
#include <string>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

 // Do Not use namesapce cv in .h file
 // or it will cause ACCESS_MASK problem (conflict with windows.h)
 // link : https://blog.csdn.net/xu20082100226/article/details/45482387

class DiePainter {
private:
	// PROPERTIES
	std::string windowName = "DieMapper";
	cv::Mat Canvas;

public:
	// constructor with default argument
	DiePainter(int rows, int cols, cv::Scalar initColor = cv::Scalar(0, 252, 124));

	// UPDATE
	// draw the selected defect dies (x, y) with dieColor
	void paint(int x, int y, cv::Scalar dieColor = cv::Scalar(0, 0, 255));
	// clear all the defect dies to initColor
	void clear(cv::Scalar initColor = cv::Scalar(0, 252, 124));

	// DISPLAY
	// display the Canvas by primitive imshow() function
	void show();
	// return the Mat of Canvas
	cv::Mat returnMat();
};