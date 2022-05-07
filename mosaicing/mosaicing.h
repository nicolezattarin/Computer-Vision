#ifndef MOSAICING_H
#define MOSAICING_H
#include <opencv2/features2d.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <map>
#include <thread>
#include <chrono>
#include "func.h"

using namespace std;
using namespace cv;
namespace fs = std::__fs::filesystem;
using std::this_thread::sleep_for;

class Mosaicing{
    public:
        Mosaicing(string dir);
        Mat GetImg (int ); //in the list
        Mat GetImg (int i, int j); //in the grid
        int GetGridSize();
        int GetNumImgs();
        vector<KeyPoint> GetKeypoints (int i); //in the list
        vector<KeyPoint> GetKeypoints (int i, int j); //in the grid
        Mat GetDescriptor (int i, int j);
        map<vector<int>, vector<DMatch>> GetMatches ();
        vector<DMatch> GetMatches (int i, int j);
        vector<vector<int>> GetPairs ();
        map<vector<int>, vector<DMatch>> GetRefinedMatches ();
        vector<DMatch> GetRefinedMatches (int i, int j);

        void SIFTdetection(int nfeatures = 100);
        void ORBdetection();
        void Matching();

        void RefineMatching(float threshold = 3);

    private:
        string m_dir;
        int m_num_images;
        int m_grid_size; //dimension of the grid of images (e.g. 3x3)
        vector<string> m_images_paths;
        vector<Mat> m_images; // list of images
        vector<vector<KeyPoint>> m_keypoints; // keypoints for each image, i.e. vector size 9
        vector<Mat> m_descriptors; // descriptors for each image, i.e. vector size 9
        map<vector<int>, vector<DMatch>> m_matches; // matches for each image, map with pair (i,j) and vector of matches
        map<vector<int>, vector<DMatch>> m_refined_matches;
        vector<vector<int>> m_pairs; // pairs of images to be matched
        bool m_sift;
        bool m_orb;
        bool m_matching;
        bool m_refined_matching;
};


#endif // MOSAICING_H