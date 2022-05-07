#ifndef FUNC_H
#define FUNC_H
#include <opencv2/features2d.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <thread>
#include <chrono>

using namespace std;
using namespace cv;
using std::this_thread::sleep_for;

void sift_detector(const Mat, Mat, vector<KeyPoint>&, Mat& );
vector<vector<Mat>> load_images(string);



    



#endif