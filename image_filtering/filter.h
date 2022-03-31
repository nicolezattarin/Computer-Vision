#ifndef FILTER_H
#define FILTER_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;
using namespace std;

class filter{
    public:
        filter(cv::Mat, int); //size : size of the kernel/window of the filter
        ~filter(){};

        void filtering();

        cv::Mat getResult();
        void setSize(int); //set the window size (square window of dimensions size x size)
        int getSize(); //get the Window Size
        void setInImage(cv::Mat);
        cv::Mat getInImage();
        cv::Mat getOutImage();

    protected:
        cv::Mat _in_image;
        cv::Mat _out_image;
        int _size;

};
// Gaussian Filter
class GaussianFilter : public filter {
    public:
        // constructor
        GaussianFilter(cv::Mat img, int size):filter(img, size){_sigma=1;};
        GaussianFilter(cv::Mat img, int size, double sigma):
                    filter(img, size){_sigma = sigma;}
        ~GaussianFilter(){};

        //setters getters
        void setSigma(double sigma){_sigma = sigma;}
        double getSigma(){return _sigma;}

        void filtering();

    private:
        double _sigma;
};

class MedianFilter : public filter {

    public:
        // constructor
        MedianFilter(cv::Mat img, int size):filter(img, size){};
        ~MedianFilter(){};

        void filtering();
};


class BilateralFilter : public filter {
    // Sigma values: For simplicity, you can set the 2 sigma values to be the same. 
    // If they are small (< 10), the filter will not have much effect,
    // whereas if they are large (> 150), they will have a very strong effect, making the image look "cartoonish".

    // Filter size: Large filters (d > 5) are very slow, so it is recommended to use d=5 
    // for real-time applications, and perhaps d=9 for offline applications that need heavy noise filtering.

    public:
        // constructor
        BilateralFilter(cv::Mat img, int size):filter(img, size){_sigma_range=1; _sigma_space=1;}
        BilateralFilter(cv::Mat img, int size, double sigma_range, double sigma_space, double pixel_diameter=9):
                        filter(img, size){_sigma_range = sigma_range; 
                                          _sigma_space = sigma_space; 
                                          _pixel_diameter = pixel_diameter;}
        ~BilateralFilter(){};

        void filtering();

        //setters getters
        void setSigmaRange(double sigma_range){_sigma_range = sigma_range;};
        double getSigmaRange(){return _sigma_range;};
        void setSigmaSpace(double sigma_space){_sigma_space = sigma_space;};
        double getSigmaSpace(){return _sigma_space;};
        void setPixelDiameter(double pixel_diameter){_pixel_diameter = pixel_diameter;};
        double getPixelDiameter(){return _pixel_diameter;};


    private:
        double _sigma_range;
        double _sigma_space;
        int _pixel_diameter;
};

#endif // FILTER_H