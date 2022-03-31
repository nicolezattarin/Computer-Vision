#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "func.h"
#include <iostream>
#include <vector>
#include "filter.h"

using namespace std;
using namespace cv;

enum FilterType { MEDIAN, GAUSSIAN, BILATERAL};
struct params{
    FilterType type;
    filter *filter;
    Mat in_image;
    string window_name;
    int size;
    int sigma1; // for gaussian is _sigma, for bilateral is _sigma_range
    int sigma2; // for bilateral is _sigma_space
};

static void on_trackbar(int, void *p) {
    params *myparams = static_cast<params*> (p);
    // filter *filter = ;
    // cout <<"size: "<<myparams->size<<endl;

    if (myparams->type == MEDIAN) { 
            cout << "MEDIAN size: " << myparams->size<< endl;
            MedianFilter* filter = static_cast<MedianFilter*>(myparams->filter);  
            filter->setSize(myparams->size);
            filter->filtering();
            imshow(myparams->window_name, filter->getOutImage());
        }
    else if (myparams->type == GAUSSIAN) {
            GaussianFilter* filter = static_cast<GaussianFilter*>(myparams->filter); 
            cout << "GAUSSIAN sigma: " << myparams->sigma1 << 
                            ", size:" << myparams->size << endl;
            filter->setSize(myparams->size);
            filter->setSigma(myparams->sigma1);
            filter->filtering();
            imshow(myparams->window_name, filter->getOutImage());
        }
    else if (myparams->type == BILATERAL) {
            BilateralFilter* filter = static_cast<BilateralFilter*>(myparams->filter);
            cout << "BILATERAL sigma1: " << myparams->sigma1 << 
                            ", sigma2: " << myparams->sigma2 << 
                            ", size: " << myparams->size << endl;
            filter->setPixelDiameter(9);
            filter->setSigmaRange(myparams->sigma1);
            filter->setSigmaSpace(myparams->sigma2);
            filter->filtering();
            imshow(myparams->window_name, filter->getOutImage());
        }
}


int main(int argc, char** argv){
    if (argc != 3){
        cout << "usage: ./main <image name> " << endl;
        return -1;}

    // Loads the image image 
	Mat img = imread(argv[1]); 
    string folder = argv[2];

    if (!img.data) {
        cout << "No image data" << endl;
        return -1;}

    // MEDIAN FILTER
    int size_slider_m = 1;
    namedWindow("median filter", WINDOW_AUTOSIZE);
    MedianFilter* median_filter = new MedianFilter(img, size_slider_m);
    params median_params = { MEDIAN, median_filter, img, "median filter", size_slider_m};
    imshow("median filter", median_filter->getInImage());
    createTrackbar("size", "median filter", &(median_params.size), 100, on_trackbar, (void*)&median_params);
    

    // GAUSSIAN FILTER
    int size_slider = 200;
    int sigma_slider = 1;
    namedWindow("gaussian filter", WINDOW_AUTOSIZE);
    GaussianFilter* gaussian_filter = new GaussianFilter(img, size_slider, sigma_slider);
    params gaussian_params = { GAUSSIAN, gaussian_filter, img, "gaussian filter", size_slider, sigma_slider};
    imshow("gaussian filter", gaussian_filter->getInImage());
    createTrackbar("size", "gaussian filter", &(gaussian_params.size), 200, on_trackbar, (void*)&gaussian_params);
    createTrackbar("sigma", "gaussian filter", &(gaussian_params.sigma1), 20, on_trackbar, (void*)&gaussian_params);

    // BILATERAL FILTER
    int size_slider_b = 1;
    int sigma_slider_b = 1;
    int sigma_slider_b2 = 1;
    namedWindow("bilateral filter", WINDOW_AUTOSIZE);
    BilateralFilter* bilateral_filter = new BilateralFilter(img, size_slider_b, sigma_slider_b, sigma_slider_b2);
    params bilateral_params = { BILATERAL, bilateral_filter, img, "bilateral filter", size_slider_b, sigma_slider_b, sigma_slider_b2};
    imshow("bilateral filter", bilateral_filter->getInImage());
    createTrackbar("size", "bilateral filter", &(bilateral_params.size), 200, on_trackbar, (void*)&bilateral_params);
    createTrackbar("sigma_range", "bilateral filter", &(bilateral_params.sigma1), 200, on_trackbar, (void*)&bilateral_params);
    createTrackbar("sigma_space", "bilateral filter", &(bilateral_params.sigma2), 200, on_trackbar, (void*)&bilateral_params);

    // wait for a key
    waitKey(0);
    return 0;
}
