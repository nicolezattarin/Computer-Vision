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

/* 
function to check if the number of images loaded is correct, 
i.e. it is a perfect square becuase we limit ourselve to only square grids
 */
bool checkperfectsquare(int n){
    if (ceil((double)sqrt(n)) == floor((double)sqrt(n))) {return true;}
    else { return false; }
}

float minDistanceMatches(vector<DMatch> matches){
    float min = 0;
    for (DMatch match : matches){
        if (min == 0){
            min = match.distance;
        }
        else if (match.distance < min){
            min = match.distance;
        }
    }
    return min;
}

vector<DMatch> MatchesRefiner(float threshold, vector<DMatch> matches){
    float minDist = minDistanceMatches(matches);

    // DEBUG
    // cout << "Min distance: " << minDist << endl;
    if (minDist == 0){
        minDist = 1;
    }
    vector<DMatch> refinedMatches;
    for (DMatch match : matches){
        if (match.distance < threshold * minDist){
            refinedMatches.push_back(match);
        }
    }
    return refinedMatches;
}

/* given a directory and a file containing thr fixed threshold,
returns a vector of threshold used to refine matches */

 map<vector<int>, float> GetThreshold(string file, string dataset, vector<vector<int>> pairs){

    map<vector<int>, float> thresholds;
    ifstream in(file);
    string line;
    // skip the first line
    getline(in, line);
    while (getline(in, line)){
        stringstream ss(line);
        string dataset_name;
        ss >> dataset_name;
        if (dataset_name == dataset){
            for (int i = 0; i<pairs.size(); i++){
                float threshold;
                ss >> threshold;
                thresholds.insert({pairs[i], threshold});
            }
        }
    }
    if (thresholds.size() == 0){
        cerr << "Error: no threshold found for dataset " << dataset << endl;
    }
    return thresholds;
}

// void translateImg(Mat& img, Mat& outimg, Point2f translation, Size s){
//     Point2f src = Point2f(0, 0);
//     Point2f dst = static_cast<Point2f> (translation);
//     Mat trans_img;
//     warpAffine(img, trans_img, getAffineTransform(&src, &dst), s);
//     outimg = trans_img.clone();
// }

//brute force
void translateImgs(vector<Mat>& images, vector<Mat>& trans_images, vector<Point2f> translations){

    float min_x, max_x, min_y, max_y;
    min_x = max_x = translations[0].x;
    min_y = max_y = translations[0].y;
    for (Point2f translation: translations) {
        min_x = min(min_x, translation.x);
        max_x = max(max_x, translation.x);
        min_y = min(min_y, translation.y);
        max_y = max(max_y, translation.y);
    }
    int n_images = images.size();
    int size_x = max_x - min_x + images[n_images-1].cols;
    int size_y = max_y - min_y + images[n_images-1].rows;
    cout << size_x << " " << size_y << endl;
    Mat t = Mat(size_y, size_x, CV_8UC1, Scalar(128,128,128));
    for (int i=0; i<n_images; i++) {
        Rect limits = Rect(translations[i].x - min_x, translations[i].y - min_y, images[i].cols, images[i].rows);
        trans_images.push_back(t(limits));
    }
    
}