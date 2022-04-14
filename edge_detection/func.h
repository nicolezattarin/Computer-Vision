#ifndef FUNC_H
#define FUNC_H

#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "func.h"
#include <iostream>
#include <fstream>
#include <map>
using namespace std;
using namespace cv;

struct CannyParams{
    int lowThreshold;
    int highThreshold;
    int sigma;
    Mat in_image;
    Mat out_image;
    string window_name;
};

struct HoughLinesParams{
    int threshold;
    int minTheta;
    int maxTheta;
    Mat in_image;
    Mat out_image;
    vector<Vec2f> out_lines;
    string window_name;
};

struct Line{ // a line is represented by two points a and b
    Point a, b;
};

int countLines(const string );
map<string, double> loadParams(const string);
vector<Point> PolarToCartesian(float, float);
vector<Vec2f> CartesianToPolar(float x, float y);
void DrawLines( Mat& , vector<Vec2f>);
void DrawInterceptionLines(Mat& img, vector<Vec2f>lines);
bool Intersection(Point2f, Point2f, Point2f, Point2f, Point2f& );
void road2Img_processing (Mat&, vector<Vec2f> );
bool isIntersecting(Point& p1, Point& p2, Point& q1, Point& q2);


static void on_trackbar_canny(int, void *p) {
    CannyParams* cparams = static_cast<CannyParams*> (p);
    Canny(cparams->in_image, 
        cparams->out_image, 
        cparams->lowThreshold, 
        cparams->highThreshold, 
        cparams->sigma);
    cout << "\nCanny params: "  << endl;
    cout << "lowThreshold: " << cparams->lowThreshold << endl;
    cout << "highThreshold: " << cparams->highThreshold << endl;
    cout << "sigma: " << cparams->sigma << endl;
    imshow(cparams->window_name, cparams->out_image);    
}

static void on_trackbar_HLines(int, void *p){
    HoughLinesParams *hparams = static_cast<HoughLinesParams*> (p);
    double rho = 1;
    double theta = CV_PI/180;

    HoughLines(hparams->in_image, hparams->out_lines, 
                rho, 
                theta, 
                hparams->threshold, 0, 0, 
                hparams->minTheta,
                hparams->maxTheta);
    cout << "\nHoughLines params: " << endl;
    cout << "threshold: " << hparams->threshold << endl;
    cout << "minTheta: " << hparams->minTheta << endl;
    cout << "maxTheta: " << hparams->maxTheta << endl;
    DrawLines(hparams->out_image, hparams->out_lines);
    imshow(hparams->window_name, hparams->out_image);    
}


#endif