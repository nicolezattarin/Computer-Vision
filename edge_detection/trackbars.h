#ifndef TRACKBARS_H
#define TRACKBARS_H

#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "utils.h"
#include "images_processing.h"
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

struct HoughCirclesParams{
    int highThreshold;
    int centerThreshold;
    int minDistance;
    int minRadius;
    int maxRadius;
    Mat in_image;
    Mat out_image;
    vector<Vec3f> out_circles;
    string window_name;
};

struct params_trackbar {
    CannyParams canny_params;
    HoughLinesParams hough_lines_params;
    HoughCirclesParams hough_circles_params;
};

static void on_trackbar_canny(int, void *p) {
    params_trackbar *params = (params_trackbar *)p;
    CannyParams* cparams = &params->canny_params;
    HoughLinesParams *hparams = &params->hough_lines_params;

    Canny(cparams->in_image, 
        cparams->out_image, 
        cparams->lowThreshold, 
        cparams->highThreshold, 
        cparams->sigma);

    cout << "\nCanny params: "  << endl;
    cout << "lowThreshold: " << cparams->lowThreshold << endl;
    cout << "highThreshold: " << cparams->highThreshold << endl;
    cout << "sigma: " << cparams->sigma << endl;

    cout << "\nHoughLines params: " << endl;
    cout << "threshold: " << hparams->threshold << endl;
    cout << "minTheta: " << hparams->minTheta << endl;
    cout << "maxTheta: " << hparams->maxTheta << endl;

    imshow(cparams->window_name, cparams->out_image);    
}

static void on_trackbar_HLines(int, void *p){
    params_trackbar *params = (params_trackbar *)p;
    CannyParams* cparams = &params->canny_params;
    HoughLinesParams *hparams = &params->hough_lines_params;   
    hparams->in_image = cparams->out_image;
    double rho = 1;
    double theta = CV_PI/180;
    Mat oldOutimg = hparams->out_image.clone();
    HoughLines(hparams->in_image, hparams->out_lines, 
                rho, 
                theta, 
                hparams->threshold, 0, 0, 
                hparams->minTheta,
                hparams->maxTheta);

    cout << "\nCanny params: "  << endl;
    cout << "lowThreshold: " << cparams->lowThreshold << endl;
    cout << "highThreshold: " << cparams->highThreshold << endl;
    cout << "sigma: " << cparams->sigma << endl;

    cout << "\nHoughLines params: " << endl;
    cout << "threshold: " << hparams->threshold << endl;
    cout << "minTheta: " << hparams->minTheta << endl;
    cout << "maxTheta: " << hparams->maxTheta << endl;

    DrawLines(hparams->out_image, hparams->out_lines);
    imshow(hparams->window_name, hparams->out_image);  
    hparams->out_image = oldOutimg;
}

static void on_trackbar_HCircles(int, void *p){
    params_trackbar *params = (params_trackbar *)p;
    HoughCirclesParams *hparams = &params->hough_circles_params;

    string window_name;
    Mat oldOutimg = hparams->out_image.clone();
    HoughCircles(hparams->in_image, hparams->out_circles, 
                HOUGH_GRADIENT, 
                1, 
                hparams->minDistance, 
                hparams->highThreshold,
                hparams->centerThreshold,
                hparams->minRadius,
                hparams->maxRadius);
    cout << "\nHoughCircle params: " << endl;
    cout << "highThreshold: " << hparams->highThreshold << endl;
    cout << "centerThreshold: " << hparams->centerThreshold << endl;
    cout << "minDistance: " << hparams->minDistance << endl;
    cout << "minRadius: " << hparams->minRadius << endl;
    cout << "maxRadius: " << hparams->maxRadius << endl;
    
    DrawCircles(hparams->out_image, hparams->out_circles);
    imshow(hparams->window_name, hparams->out_image); 
    hparams->out_image = oldOutimg;
}

#endif