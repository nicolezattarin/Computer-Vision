#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include "utils.h"
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include <map>
using namespace std;
using namespace cv;

/* SPECIFIC PROCESSING FOR EACH IMAGE TAKEN INTO ACCOUNT */

// ROAD2
void road2Img_processing (Mat&, vector<Vec2f>); 


#endif