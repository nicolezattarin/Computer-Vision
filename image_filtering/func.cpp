#include "func.h"


// shows the image
void showImage(cv::Mat& img, string window_name){
    namedWindow(window_name, WINDOW_AUTOSIZE);
    imshow(window_name, img);
}


void saveImage(cv::Mat& img, string path){
    if(img.empty()) cerr << "Imgage is empty." << endl;
    cv::imwrite(path, img); 
}