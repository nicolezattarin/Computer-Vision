#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/features2d.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <thread>
#include <chrono>

using namespace std;
using namespace cv;
using std::this_thread::sleep_for;

string WINDOW_RESULT = "Result";
string WINDOW_KP = "Keypoints";

int main(int argc, char** argv)
{
    Mat img1 = imread("../lab/LNSRT4/lab_patch00.jpg");
    Mat img2 = imread("../lab/LNSRT4/lab_patch01.jpg");

    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    Ptr<ORB> detector = ORB::create();
    detector->detect(img1, keypoints1);
    detector->compute(img1, keypoints1, descriptors1);
    detector->detect(img2, keypoints2);
    detector->compute(img2, keypoints2, descriptors2);

    Mat img_keypoints1;
    drawKeypoints(img1, keypoints1, img_keypoints1);
    imshow(WINDOW_KP, img_keypoints1);
    waitKey(0);

    Mat img_keypoints2;
    drawKeypoints(img2, keypoints2, img_keypoints2);
    imshow(WINDOW_KP, img_keypoints2);
    waitKey(0);

    
    return 0;
}




