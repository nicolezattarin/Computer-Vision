
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
    // test string management
    string test = "lab_patch00.jpg";
    int index_point = test.find_last_of(".");
    cout << index_point << endl;
    cout << stoi(test.substr(index_point-2,index_point-1)) << endl;
    cout << stoi(test.substr(index_point-1,index_point)) << endl;

    cout << "OpenCV version : " << CV_VERSION << endl;

    Mat img1 = imread("padova_patch01.jpg");
    Mat img2 = imread("padova_patch02.jpg");

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
    // imshow(WINDOW_KP+"1", img_keypoints1);

    Mat img_keypoints2 = img2.clone();
    for (int k = 0; k < keypoints2.size(); k++){circle(img_keypoints2, keypoints2[k].pt, 5, Scalar(0,0,255), 2);}
    // imshow(WINDOW_KP+"2", img_keypoints2);

    // Match the descriptors
    // imshow("des1", descriptors1);
    // imshow("des2", descriptors2);

    // waitKey(0);
    // Matching descriptor vectors using BFMatcher
    BFMatcher matcher(NORM_HAMMING);
    vector<DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);

    cout << "Number of matches : " << matches.size() << endl;
    vector <float>  distances;
    for (int i = 0; i < matches.size(); i++){
        distances.push_back(matches[i].distance);
    }
    // float min_dist = *min_element(distances.begin(), distances.end());
    float min_dist = matches[0].distance;
    for (int i = 0; i<matches.size(); i++){
        cout << "distances " << matches[i].distance << endl;
        if (min_dist > matches[i].distance && matches[i].distance > 0.0){
            min_dist = matches[i].distance;
        }
    }
    cout << "Minimum distance : " << min_dist << endl;
    vector<DMatch> good_matches;
    float threshold = 0.1;
    for (int i = 0; i<matches.size(); i++){
        if (matches[i].distance < min_dist*threshold){
            good_matches.push_back(matches[i]);
        }
    }
    cout << "Number of good matches : " << good_matches.size() << endl;

    Mat matchesOut;
    drawMatches	(img1, keypoints1, img2, keypoints2, good_matches, matchesOut, Scalar(0,0,255), Scalar(0,255,0));
    imwrite("matches.png", matchesOut);


    return 0;
}



