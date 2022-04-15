#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "func.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <thread>
#include <chrono>

using namespace std;
using namespace cv;
using std::this_thread::sleep_for;

double RHO_CELL_SIZE = 1;
double THETA_CELL_SIZE = CV_PI/180;
double DP_HCIRCLES = 1;

string WINDOW_EDGES = "Edge Map";
string WINDOW_ORIGINAL = "Original image";
string WINDOW_LINES = "Line detection";
string WINDOW_FILLED_LINES = "Filled line detection";
string WINDOW_FILLED_CIRCLES = "Filled lines-circles detection";


int main(int argc, char** argv)
{
    /**********************************************************
     *              LOADING PARAMETERS FROM FILE              *
     * *******************************************************/

    if (argc != 6){
        cout << "Usage: " << argv[0] << " <image_name> <params file path> <tune canny> <tune HoughLines> <tune HoughCircles>" << endl;
        return -1;}
    string pathImage = argv[1];
    // get filename
    string dir = "images/";
    size_t pos = pathImage.find("images/")+dir.length();;      
    string fileName = pathImage.substr (pos);

    //get other cl args 
    string pathParams = argv[2];
    bool tuneCanny = atoi(argv[3]);
    bool tuneHLines = atoi(argv[4]);
    bool tuneHCircles = atoi(argv[5]);
    cout << "Image path: " << pathImage << endl;
    cout << "tune Canny: " << tuneCanny << endl;
    cout << "tune HoughLines: " << tuneHLines << endl;

    // load parameters from file
    int nparams = countLines(pathParams);
    map<string, double> params;
    params = loadParams(pathParams);
    cout << "\n\nParameters: " << nparams << endl;
    for (auto it = params.begin(); it != params.end(); it++)
        cout << it->first << " " << it->second << endl;
    
    /**********************************************************
     *                      ORIGINAL IMAGE                    *
     * *******************************************************/

    // Loads the image image 
	Mat img = imread(pathImage); 
    if (!img.data) {
        cout << "\nNo image data" << endl;
        return -1;}

    // show imag
    namedWindow(WINDOW_ORIGINAL, WINDOW_AUTOSIZE);   
    imshow(WINDOW_ORIGINAL, img);

    /**********************************************************
                    EDGE DETECTION 
    ***********************************************************/
    // Generate the edge map with the Canny algorithm. 
    Mat edgeMap;
    namedWindow(WINDOW_EDGES, WINDOW_AUTOSIZE);  
    // work on the grey scale image
    Mat greyImg;
    cvtColor(img, greyImg, COLOR_BGR2GRAY);

    // if we tune the parameters we only work on that, 
    // to perform all the steps set the tuning flags to false
    if (tuneCanny == 1){
        // setup params
        CannyParams cParams = {static_cast<int>(params["lowThreshold_canny"]), 
                                static_cast<int>(params["highThreshold_canny"]), 
                                static_cast<int>(params["sigma_canny"]), 
                                greyImg, 
                                edgeMap, 
                                WINDOW_EDGES};

        imshow(WINDOW_EDGES, greyImg);
        cout << "\nWarning: you can tune the lower and higher thresholds with the trackbars" << endl;
        cout << "While the sigma is fixed in the file, sigma must be between 3 and 7" << endl;
        createTrackbar("lowThreshold", WINDOW_EDGES, &(cParams.lowThreshold), 500, on_trackbar_canny, (void*)&cParams);
        createTrackbar("highThreshold", WINDOW_EDGES, &(cParams.highThreshold), 800, on_trackbar_canny, (void*)&cParams);
        waitKey(0);
        return 0;
    } 
    else {
        Canny(greyImg, 
            edgeMap, 
            static_cast<int>(params["lowThreshold_canny"]), 
            static_cast<int>(params["highThreshold_canny"]), 
            static_cast<int>(params["sigma_canny"]));
        imshow(WINDOW_EDGES, edgeMap);
        imwrite("results/edgeMap_"+fileName, edgeMap);
    }

    /**********************************************************
     *               HOUGH LINES DETECTION                    *
     * *******************************************************/

    // Use the Edge Map as input for the standard Hough transform. 
    vector<Vec2f> lines; //Array of 2-elements vectors (ρ,θ)
    Mat LinesImg = img.clone();
    Mat filledLinesImg = img.clone();
    namedWindow(WINDOW_LINES, WINDOW_AUTOSIZE);  

    if (tuneHLines == 1){
        imshow( WINDOW_LINES, LinesImg);
        HoughLinesParams hlParams = {static_cast<int>(params["threshold_HoughLines"]), 
                                 static_cast<int>(params["minTheta_HoughLines"]), 
                                 static_cast<int>(params["maxTheta_HoughLines"]), 
                                 edgeMap,
                                 LinesImg,
                                 lines, 
                                 WINDOW_LINES};
        cout << "\nWarning: you can tune the threshold, minimum and maximum angle" << endl;
        cout << "While the discretization of the grid is fixed by a global variable" << endl;
        createTrackbar("threshold", WINDOW_LINES, &(hlParams.threshold), 700, on_trackbar_HLines, (void*)&hlParams);
        createTrackbar("minTheta", WINDOW_LINES, &(hlParams.minTheta), 360, on_trackbar_HLines, (void*)&hlParams);
        createTrackbar("maxTheta", WINDOW_LINES, &(hlParams.maxTheta), 360, on_trackbar_HLines, (void*)&hlParams);
        waitKey(0);
        return 0;
      }
    else{
        HoughLines(edgeMap, lines, 
                        RHO_CELL_SIZE, 
                        THETA_CELL_SIZE, 
                        params["threshold_HoughLines"], 0, 0, 
                        params["minTheta_HoughLines"],
                        params["maxTheta_HoughLines"]);
        cout << "\n\n" << lines.size() << " lines detected" << endl;
        
        DrawInterceptionLines(LinesImg, lines);
        imshow(WINDOW_LINES,LinesImg);
        imwrite("results/Lines_"+fileName, LinesImg);

        // FILLED VERSION
        road2Img_processing (filledLinesImg, lines);
        namedWindow(WINDOW_FILLED_LINES, WINDOW_AUTOSIZE);  
        imshow(WINDOW_FILLED_LINES, filledLinesImg);
        imwrite("results/FilldEdgeMap_"+fileName, filledLinesImg);
    }
    

    /**********************************************************
    *               HOUGH CIRCLE DETECTION                   *
    * *******************************************************/

    vector<Vec3f> circles; //each element is a 3-element floating-point vector (x, y, radius)
    Mat circlesImg = filledLinesImg.clone(); //start from filled lines version
    namedWindow(WINDOW_FILLED_CIRCLES, WINDOW_AUTOSIZE);  
    medianBlur(greyImg, greyImg, 3);
    cout << "\nMedian Blur done" << endl;


    if (tuneHCircles == 1){
        imshow(WINDOW_FILLED_CIRCLES, circlesImg);
        HoughCirclesParams hcParams = {static_cast<int>(params["highThreshold_HoughCircles"]), 
                                 static_cast<int>(params["centerThreshold_HoughCircles"]), 
                                 static_cast<int>(params["minDist_HoughCircles"]),
                                 static_cast<int>(params["minRad_HoughCircles"]), 
                                 static_cast<int>(params["maxRad_HoughCircles"]), 
                                 greyImg,
                                 circlesImg, 
                                 circles,
                                 WINDOW_FILLED_CIRCLES};
        createTrackbar("high threshold", WINDOW_FILLED_CIRCLES, &(hcParams.highThreshold), 200, on_trackbar_HCircles, (void*)&hcParams);
        createTrackbar("center threshold", WINDOW_FILLED_CIRCLES, &(hcParams.centerThreshold), 200, on_trackbar_HCircles, (void*)&hcParams);
        createTrackbar("min distance", WINDOW_FILLED_CIRCLES, &(hcParams.minDistance), 10, on_trackbar_HCircles, (void*)&hcParams);
        createTrackbar("min radius", WINDOW_FILLED_CIRCLES, &(hcParams.minRadius), 10, on_trackbar_HCircles, (void*)&hcParams);
        createTrackbar("max radius", WINDOW_FILLED_CIRCLES, &(hcParams.maxRadius), 100, on_trackbar_HCircles, (void*)&hcParams);

        waitKey(0);
        return 0;
      }
    else{
        // work on the grey scale image
        HoughCircles(greyImg, circles, HOUGH_GRADIENT, 
                        1, //1 = dp = 1
                        (params["minDist_HoughCircles"]), 
                        (params["highThreshold_HoughCircles"]), 
                        (params["centerThreshold_HoughCircles"]), 
                        (params["minRad_HoughCircles"]), 
                        (params["maxRad_HoughCircles"]));
        cout << "\n\n" << circles.size() << " circles detected" << endl;
        DrawFilledCircles(circlesImg, circles);
        imshow(WINDOW_FILLED_CIRCLES, circlesImg);
        imwrite("results/Circles_"+fileName, circlesImg);

    }
    


    waitKey(0);
    return 0;
}
