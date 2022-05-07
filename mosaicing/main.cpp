
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
#include "mosaicing.h"

using namespace std;
using namespace cv;
using std::this_thread::sleep_for;

string WINDOW_RESULT = "Result";
string WINDOW_KP = "Keypoints";

int main(int argc, char** argv)
{
    /**********************************************************
     *                      LOADING IMAGES                    *
     * *******************************************************/

    /*
    Load a set of images from one of the provided datasets.
    The images are roughly arranged in a 3x3 grid and have been obtained by s
    plitting a larger image and applying some transformations, each trasformation is denoted by a letter:
    “T” stays for translation, 
    “R” rotation, 
    “S” scaling, 
    “N” noise, 
    “L” illumination change 
    */
    cout << "OpenCV version : " << CV_VERSION << endl;

    if (argc != 2){
        cout << "Usage: " << argv[0] << " <path to image folder>" << endl;
        return -1;}
    string dir = argv[1];

    Mosaicing mosaic(dir);
    int grid_size = mosaic.GetGridSize();
    int num_images = mosaic.GetNumImgs();
    cout << "Grid size: " << grid_size << endl;
    cout << "Num images: " << num_images << endl;

    //DEBUG: show the images
    // for (int i = 0; i < grid_size; i++){
    //     for (int j = 0; j < grid_size; j++){
    //         namedWindow("Image " + to_string(i) + to_string(j), WINDOW_NORMAL);
    //         imshow("Image " + to_string(i) + to_string(j), mosaic.GetImg(i,j));
    //         }
    //     }
    // waitKey(0);

    /**********************************************************
     *                     KP DETECTION                       *
     * *******************************************************/

    mosaic.ORBdetection();
    
    // DrawKeypoints
    for (int i = 0; i < grid_size; i++){
        for (int j = 0; j < grid_size; j++){
            Mat img = mosaic.GetImg(i,j);
            vector<KeyPoint> KP = mosaic.GetKeypoints(i,j);
            drawKeypoints(img, KP, img, Scalar(0,255,0));
            imwrite("KPs/"+dir.substr (dir.find("/"))+"kp_" + to_string(i) + to_string(j) + ".png", img);
        }
    }
    // DEBUG: show the descriptors
    for (int i = 0; i < grid_size; i++){
        for (int j = 0; j < grid_size; j++){
            Mat des = mosaic.GetDescriptor(i,j);
            cout << "Descriptor " << i << " " << j << " size: " << des.size() << endl;
            imwrite("Descriptor/"+dir.substr (dir.find("/"))+"kp_" + to_string(i) + to_string(j) + ".png", des);
        }
    }
    /**********************************************************
    *                       MATCHING                          *
    * ********************************************************/
    mosaic.Matching();

    //DEBUG: show the matches
    map <vector<int>, vector<DMatch>> PairMatches = mosaic.GetMatches();
    cout << "number of pairs matched " << PairMatches.size() << endl;         

    // refine the matches
    mosaic.RefineMatching();

    //Draw Matches
    vector<vector<int>> pairs = mosaic.GetPairs ();
    for (int p = 0; p<pairs.size(); p++){
        int index1 = pairs[p][0];
        int index2 = pairs[p][1];
        Mat out;
        drawMatches	(mosaic.GetImg(index1), mosaic.GetKeypoints(index1),
                    mosaic.GetImg(index2), mosaic.GetKeypoints(index2),
                    mosaic.GetRefinedMatches(index1, index2), out,
                    Scalar(0,255,0), Scalar(0,255,0));
        imwrite("Matches/"+dir.substr (dir.find("/"))+"_matches_" + to_string(index1) + to_string(index2) + ".png", out);
    }

    /**********************************************************
     *                 AFFINE TRANFORMATION                   *
     * *******************************************************/
    // images are linked together by an affine transform, find the transformation between the images. 
    //we use RANSAC algorithm. The set of inliers can be computed by using the findHomography() 
    // function, with CV_RANSAC as the third parameter 
    // (hint: the inliers can be retrieved by using the mask argument).


    return 0;
}



