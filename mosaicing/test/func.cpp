#include "func.h"

vector<vector<Mat>> load_images(string dir){
    vector<vector<Mat>> images;
    for (int i = 0; i < 3; i++){
            vector<Mat> row;
            for (int j = 0; j < 3; j++){
                string path = dir + "/lab_patch" + to_string(i) + to_string(j) + ".jpg";
                Mat img = imread(path);
                cout << "Loading imge " << i << j << ", size " << img.size() << endl;
                if (img.empty()){
                    cerr << "Error: image " << path << " not found" << endl;}
                row.push_back(img);
            }
            images.push_back(row);
        }
    return images;
 }

  
// void sift_detector(const Mat img, Mat mask, vector&<KeyPoint> keypoints){
//     Ptr<SIFT> sift = SIFT::create();
//     sift->detect(img, keypoints); 
//     vector<Mat> descriptors;
//     sift->compute(imaimgges, keypoints, descriptors);
// }