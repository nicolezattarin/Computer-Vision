#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "func.h"
#include <iostream>
using namespace std;
using namespace cv;


int main(int argc, char** argv)
{
    if (argc != 5){
        cout << "usage: test <image_name> thresholds <TH B> <TH G> <TH R>" << endl;
        return -1;}

	Mat img = imread(argv[1]); // read the image

    if (!img.data) {
        cout << "No image data" << endl;
        return -1;}

    // General info
    cout << "image size: " << img.size() << endl;
    cout << "image type: " << img.type() << endl;
    cout << "image channels: " << img.channels() << endl;
    cout << "image depth: " << img.depth() << endl;
    cout << "image cols: " << img.cols << endl;
    cout << "image rows: " << img.rows << endl;
    
    // create a window
    namedWindow("MyWindow", WINDOW_AUTOSIZE);   
    //capture a pixel position when mouse is clicked
    pixel px;
    cout << "Click on the image to capture the pixel position" << endl;
    cout << "click on the color that you want to segment" << endl;
    setMouseCallback("MyWindow", click, &px); 
    // note debug: always set mouse callback before imshow, 
    // not when the image is shown 

    // Shows the image on a window
    cout << "Press any key on the window to close the image" << endl;
    imshow("MyWindow", img); 
    // wait for a click on the window befor closing and computing the mean color
    while (px.l_pressed == false){
        waitKey(1);}
    destroyWindow("MyWindow");

    // subimage
    square_subimage (img, px.x, px.y, 9, false);
        
    // Computes the mean RGB color over a 9x9 neighborhood of the clicked point
    // remind default is BGR 
    double mblue, mgreen, mred;
    mean_color(img, px.x, px.y, 9, mblue, mgreen, mred);
    Vec3b mean = Vec3b(mblue, mgreen, mred);

    // Segment the soccer shirts by applying a static threshold to the three channels R, G 
    // and B (e.g., ΔR < 50, ΔG < 50, ΔB < 50, but try to change the value)
    int B_threshold = atoi(argv[2]);
    int G_threshold = atoi(argv[3]);
    int R_threshold = atoi(argv[4]);

    Mat img_seg;
    img.copyTo(img_seg);
    for (int i = 0; i < img.rows; i++){
        for (int j = 0; j < img.cols; j++){
            Vec3b color = img.at<Vec3b>(i, j);
            // of the color has components which are close to the mean value selected, we set the pixel to (37,201,92)
            if (abs(color[0]-mean[0])< B_threshold || abs(color[1]-mean[1]) < G_threshold || abs(color[2]-mean[2]) < R_threshold){
                img_seg.at<Vec3b>(i, j) = Vec3b(37,201,92);}
        }
    }
    namedWindow("segment_img", WINDOW_AUTOSIZE);
    imshow("segment_img", img_seg);
    waitKey(0);

	destroyAllWindows(); // Destroy all the windows
    return 0;
}
