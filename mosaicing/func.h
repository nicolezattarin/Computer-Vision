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

vector<vector<Mat>> load_images(string);
bool checkperfectsquare(int);

/*
gets a vector of vectors (i.e. matches for each pair)
and returns the minimum distance between matches over all pairs of images
*/
float minDistanceMatches(vector<DMatch>); 
vector<DMatch> MatchesRefiner(float, vector<DMatch>);
map<vector<int>, float> GetThreshold(string, string, vector<vector<int>> );
void translateImgs(vector<Mat>& images, vector<Mat>& trans_images, vector<Point2f> translations);



#endif