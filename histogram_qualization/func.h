#ifndef FUNC_H
#define FUNC_H

#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "func.h"
#include <iostream>
using namespace std;
using namespace cv;

// plots the histograms of the image
void showHistogram(std::vector<cv::Mat>&, string, bool); 

// shows the image
void showImage(cv::Mat&, string);
// equalize image
void equalize(cv::Mat&, cv::Mat&, std::vector<cv::Mat>&, std::vector<cv::Mat>&, string);

// save image to path
void saveImage(cv::Mat&, string);

#endif