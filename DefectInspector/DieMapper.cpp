#include "DieMapper.h"

#include <iostream>

using namespace std;
using namespace cv;

//---------------------------------------------------------------------

cv::Scalar DieMapper::WarningLevel(int defectNum) {
    // return different color depends on the #defect
    // depends on the ratio of (defect pixels / total pixels)
    // have 5 different levels, from green to red gradually

    switch (defectNum * 10 / 810000) {  // TODO :   replaced with other variables
    case 0:
    case 1:
        return  cv::Scalar(209, 209, 255);  // TODO : should declare these colors in Global
        break;
    case 2:
    case 3:
        return cv::Scalar(82, 82, 255);
        break;
    case 4:
        return cv::Scalar(0, 0, 255);
        break;
    default:
        return cv::Scalar(0, 252, 124);
        break;
    }
    return cv::Scalar(0, 0, 255);
}

void DieMapper::dividerLine() {
    // draw the divider line on the Canvas to help regonize the regions
    // should call this function in each DISPLAY function.

    // Vertical dividerLine
    for (int c = 0; c < COLS + 1; c++) {
        cv::Point from = cv::Point(c * regionLength, 0);
        cv::Point dest = cv::Point(c * regionLength, ROWS * regionLength);

        line(Canvas, from, dest, cv::Scalar(0, 0, 0));
    }

    // Horizontal dividerLine
    for (int r = 0; r < ROWS + 1; r++) {
        cv::Point from = cv::Point(0, r * regionLength);
        cv::Point dest = cv::Point(COLS * regionLength, r * regionLength);

        line(Canvas, from, dest, cv::Scalar(0, 0, 0));
    }
}

//---------------------------------------------------------------------

DieMapper::DieMapper(int rows, int cols, cv::Scalar initColor) {
    // constructor with default argument
    // will create a (cv::Mat) Canvas and initialize it to initColor
    this->Canvas = cv::Mat::zeros(cv::Size(cols * regionLength, rows * regionLength), CV_8UC3);
    (this->Canvas).setTo(initColor);
}

//---------------------------------------------------------------------

void DieMapper::paint(int region, int defectNum) {
    // draw the selected region by its WarningLevel(depends on #defects)

    int c = (region % COLS) * regionLength;
    int r = (region / COLS) * regionLength;
    cv::Scalar Color = WarningLevel(defectNum);

    rectangle(Canvas, cv::Point(c, r), cv::Point(c + regionLength, r + regionLength), Color, cv::FILLED);
}

//---------------------------------------------------------------------

void DieMapper::show() {
    // display the Canvas by primitive imshow() function
    // for debug  use
    imshow(windowName, Canvas);
}

cv::Mat DieMapper::returnMat() {
    // return the Mat of Canvas
    // it will be later transform to Bitmap^ to show on the clr::pictureBox

    dividerLine();
    return Canvas;
}

//---------------------------------------------------------------------