
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "func.h"
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
    
    cout << "OpenCV version : " << CV_VERSION << endl;

    Mat img1 = imread("lab_patch00.jpg");
    Mat img2 = imread("lab_patch01.jpg");

    // Detect the keypoints and extract descriptors
    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    Ptr<ORB> detector = ORB::create();
    detector->detect(img1, keypoints1);
    detector->compute(img1, keypoints1, descriptors1);
    detector->detect(img2, keypoints2);
    detector->compute(img2, keypoints2, descriptors2);

    Mat img_keypoints1 = img1.clone();
    for (int k = 0; k < keypoints1.size(); k++){circle(img_keypoints1, keypoints1[k].pt, 5, Scalar(0,0,255), 2);}
    imshow(WINDOW_KP, img_keypoints1);

    Mat img_keypoints2 = img2.clone();
    for (int k = 0; k < keypoints2.size(); k++){circle(img_keypoints2, keypoints2[k].pt, 5, Scalar(0,0,255), 2);}
    imshow(WINDOW_KP, img_keypoints2);

    // Match the descriptors
    imshow("des1", descriptors1);
    imshow("des2", descriptors2);
    waitKey(0);
    // Matching descriptor vectors using BFMatcher
    BFMatcher matcher(NORM_HAMMING);
    vector<DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);

    cout << "Number of matches : " << matches.size() << endl;
    for (int i = 0; i<matches.size(); i++){
        cout << "distances " << matches[i].distance << endl;
    }




    return 0;
}



