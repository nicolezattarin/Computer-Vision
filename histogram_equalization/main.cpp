#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "func.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
    if (argc != 3){
        cout << "usage: ./main <image name> <save folder>" << endl;
        return -1;}

    // Loads the image image 
	Mat img = imread(argv[1]); 
    string folder = argv[2];

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
    showImage(img, "Original image");

    // RGB
    Mat equalized_img;
    vector<Mat> hists(3);
    vector<Mat> equalized_hists(3);
    equalize(img, equalized_img, hists, equalized_hists, "RGB");
    showHistogram(hists, folder+"/original_histo", true);
    showImage(equalized_img, "Equalized image RGB");
    showHistogram(equalized_hists, folder+"/histo_eq_image_RGB", true);
    saveImage(equalized_img, folder+"/eq_image_RGB.png");

    // BGR2HSV
    img = imread(argv[1]);
    Mat BGR2HSV_equalized_img;
    vector<Mat> BGR2HSV_hists(3);
    vector<Mat> BGR2HSV_equalized_hists(3);
    equalize(img, BGR2HSV_equalized_img, BGR2HSV_hists, BGR2HSV_equalized_hists, "BGR2HSV");
    showImage(BGR2HSV_equalized_img, "Equalized image BGR2HSV");
    showHistogram(BGR2HSV_equalized_hists, folder+"/histo_eq_image_BGR2HSV", true);
    saveImage(BGR2HSV_equalized_img, folder+"/eq_image_BGR2HSV.png");

    // BGR2Lab
    img = imread(argv[1]);
    Mat BGR2Lab_equalized_img;
    vector<Mat> BGR2Lab_hists(3);
    vector<Mat> BGR2Lab_equalized_hists(3);
    equalize(img, BGR2Lab_equalized_img, BGR2Lab_hists, BGR2Lab_equalized_hists, "BGR2Lab");
    showImage(BGR2Lab_equalized_img, "Equalized image HSBGR2Lab");
    showHistogram(BGR2Lab_equalized_hists, folder+"/histo_eq_image_BGR2Lab", true);
    saveImage(BGR2Lab_equalized_img, folder+"/eq_image_HSBGR2Lab.png");


    // wait for a key
    waitKey(0);
    return 0;
}
