#include "DiePainter.h"

#include <iostream>

using namespace std;
using namespace cv;

//---------------------------------------------------------------------

DiePainter::DiePainter(int rows, int cols, cv::Scalar initColor) {
	// constructor with default argument
	// will create a (cv::Mat) Canvas and initialize it to initColor

	this->Canvas = cv::Mat::zeros(cv::Size(cols, rows), CV_8UC3);
	(this->Canvas).setTo(initColor);
}

//---------------------------------------------------------------------

void DiePainter::paint(int x, int y, cv::Scalar dieColor) {
	// draw the selected defect dies (x, y) with dieColor
	// draw the die in circle with radius 1 and solid
	circle(Canvas, cv::Point(x, y), 1, dieColor, -1);
}

void DiePainter::clear(cv::Scalar initColor) {
	// clear all the defect dies to initColor
	// often be called when the region in Mapper changing
	(this->Canvas).setTo(initColor);
}

//---------------------------------------------------------------------

void DiePainter::show() {
	// display the Canvas by primitive imshow() function
	// for debug use
	imshow(windowName, Canvas);
}

cv::Mat DiePainter::returnMat() {
	// return the Mat of Canvas
	return Canvas;
}

//---------------------------------------------------------------------