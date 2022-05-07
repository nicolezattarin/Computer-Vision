#include "mosaicing.h"

Mosaicing :: Mosaicing(string dir){
    m_dir = dir;
    m_num_images = 0;
    for (const auto & entry : fs::directory_iterator(dir)){
        string extension = entry.path().u8string().substr(entry.path().u8string().find_last_of(".") + 1);
        if (extension == "jpg" || extension == "png" || extension == "jpeg"){
            m_images_paths.push_back(entry.path());
            m_num_images ++;
            m_images.push_back(imread(entry.path()));
            if (m_images.back().empty()){
                cerr << "Error: image " << entry.path() << " not found" << endl;
                m_num_images --;
            }
        }
    }
    m_grid_size = sqrt(m_num_images);

    // find pairs of images to be matched
    for (int n = 0; n < m_num_images-1; n++){
        if (n<m_num_images-m_grid_size-1) {
            m_pairs.push_back({n, n+m_grid_size});
        }
        if (n%m_grid_size != 0) {
            m_pairs.push_back({n-1, n});
        }
    }

    m_sift = false;
    m_orb = false;
    m_matching = false;
    m_refined_matching = false;
}

Mat Mosaicing :: GetImg (int i){
    return m_images[i];
}

Mat Mosaicing :: GetImg (int i, int j){
    return m_images[i*m_grid_size  + j];
}

int Mosaicing :: GetGridSize(){
    return m_grid_size;
}

int Mosaicing :: GetNumImgs(){
    return m_num_images;
}

vector<KeyPoint> Mosaicing :: GetKeypoints(int i){
    return m_keypoints[i];
}

vector<KeyPoint> Mosaicing :: GetKeypoints(int i, int j){
    return m_keypoints[i*m_grid_size + j];
}

Mat Mosaicing :: GetDescriptor(int i, int j){
    return m_descriptors[i*m_grid_size + j];
}

map <vector<int>, vector<DMatch>> Mosaicing :: GetMatches(){
    return m_matches;
}

vector<DMatch> Mosaicing :: GetMatches(int i, int j){
    vector <int> pair = {i, j};
    return m_matches.at(pair);
}

vector<vector<int>> Mosaicing :: GetPairs(){
    return m_pairs;
}

map<vector<int>, vector<DMatch>> Mosaicing :: GetRefinedMatches(){
    return m_refined_matches;
}

vector<DMatch> Mosaicing :: GetRefinedMatches(int i, int j){
    vector <int> pair = {i, j};
    return m_refined_matches.at(pair);
}

void Mosaicing :: SIFTdetection(int nfeatures){
    cout << "\nSIFT detection" << endl;
    vector<KeyPoint> KPTemp;
	Mat descriptorTemp;

	for (int i = 0; i < m_num_images; i++) {
        cout << "Detecting keypoints for image " << i << " of size " << m_images[i].size() << endl;
        // sift
        Ptr<SIFT> detector = SIFT::create(nfeatures);
		detector->detect(m_images[i], KPTemp);
        detector->compute(m_images[i], KPTemp, descriptorTemp);
		m_keypoints.push_back(KPTemp);
		m_descriptors.push_back(descriptorTemp);
		KPTemp.clear(); 
    }
    m_sift = true;
    m_orb = false; // if sift is detected, orb is not used
                   // needed to choose the correct distance in Matching()
    cout << "\nSIFT features extracted\n" << endl;
}

void Mosaicing :: ORBdetection(){
    cout << "\nORB detection" << endl;
    vector<KeyPoint> KPTemp;
    Mat descriptorTemp;
    for (int i = 0; i < m_num_images; i++) {
        cout << "Detecting keypoints for image " << i << " of size " << m_images[i].size() << endl;
        // orb
        Ptr<ORB> detector = ORB::create();
        detector->detectAndCompute(m_images[i], Mat(), KPTemp, descriptorTemp);
        m_keypoints.push_back(KPTemp);
        m_descriptors.push_back(descriptorTemp);
        KPTemp.clear();
    }
    m_orb = true;
    m_sift = false; // if orb is used, sift keypoints are gonna be ord and not sift, 
                    // this is needed for matching with the right distance
    cout << "\nORB features extracted\n" << endl;
}

void Mosaicing :: Matching(){
    cout << "\nMatching" << endl;
    // DEBUG
    cout << "descriptors size: " << m_descriptors.size() << endl;
    // check if sift or orb is used
    // L2 distance for SIFT and the Hamming distance for ORB.

    //DEBUG: the pointer gives a run time segmentation fault 11, in first version we use variable
    BFMatcher matcher;
    if (!m_sift && !m_orb){
        cerr << "Error: detection not performed, detect keypoints before performing matching" << endl;
        return;
    }
    if (m_sift){
        cout << "Matching keypoints with SIFT" << endl;
        matcher = BFMatcher(NORM_L2);
    }
    else if (m_orb){
        cout << "Matching keypoints with ORB" << endl;
        matcher = BFMatcher(NORM_HAMMING);
    }
    for (int n = 0; n < m_pairs.size(); n++){
        vector<DMatch> matches;
        vector<int> pair = m_pairs[n];
        matcher.match(m_descriptors[pair[0]], m_descriptors[pair[1]], matches);
        m_matches.insert({pair, matches});
        matches.clear();
        cout << "Matching (" << pair[0] << ", " << pair[1] << ") matchsize: " << m_matches.at(pair).size() << endl;
    }
    // DEBUG print matches
    // for (int n = 0; n < m_pairs.size(); n++){
    //     vector<int> pair = m_pairs[n];
    //     cout << "distance between (" << pair[0] << ", " << pair[1] << ") is: " << endl;
    //     vector<DMatch> mm = m_matches.at(pair);
    //     for (int i = 0; i < mm.size(); i++){
    //         cout << mm[i].distance << ", ";
    //     }
    //     cout << endl;
    // }
    m_matching = true;
    cout << "Matching done\n" << endl;
}

void Mosaicing :: RefineMatching(float threshold){
    // Refine by selecting the matches with distance less
    // than threshold * min_distance,for each pair

    cout << "\nRefining matching" << endl;
    if (!m_matching){
        cerr << "Error: matching not performed, perform matching before refining" << endl;
        return;
    }
     // get the minimum distance and select
    for (int n = 0; n < m_pairs.size(); n++){
        vector<DMatch> mm = m_matches.at(m_pairs[n]);
        vector<DMatch> refined = RefineMatches(threshold, mm);
        m_refined_matches.insert({m_pairs[n], refined});
        cout << "Refined pair (" << m_pairs[n][0] << ", " << m_pairs[n][1] << ") oldsize " << mm.size() << " newsize " << refined.size() << endl;
    }
    m_refined_matching = true;
    cout << "Matching refined\n" <<endl;
}

    
	
