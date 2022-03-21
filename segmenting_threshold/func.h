#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
using namespace std;
using namespace cv;

struct pixel{
    int x;
    int y;
    bool l_pressed;
    bool r_pressed;
};

// gets the pixel position when mouse is clicked
// void click(int event, int x, int y, int flags, void *userdata);
void click(int, int, int, int, void*);

// computes the mean RGB color over a sizexsize neighborhood of the clicked point
void mean_color(Mat, int, int, int, double&, double&, double&);

// gets the subimage of the image centered at the clicked pixel
void square_subimage (Mat, int, int, int, bool);

// apply a filter to the image
void static_threshold_segmentation (Mat, int* , int* , Mat&, int*, bool);