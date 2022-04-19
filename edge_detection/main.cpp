#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "utils.h"
#include "images_processing.h"
#include "trackbars.h"
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
string WINDOW_CIRCLES = "Circles detection";



int main(int argc, char** argv)
{
    /**********************************************************
     *              LOADING PARAMETERS FROM FILE              *
     * *******************************************************/

    if (argc != 5){
        cout << "Usage: " << argv[0] << " <image path> <params file path> <tune> <save>" << endl;
        cout << "Warning: if save is 1, any existing file will be overwritten" << endl;
        return -1;}
    string pathImage = argv[1];
    // get filename
    string dir = "images/";
    size_t pos = pathImage.find("images/")+dir.length();;      
    string fileName = pathImage.substr (pos);
    size_t lastindex = fileName.find_last_of("."); 
    fileName = fileName.substr(0, lastindex); 
    bool tune = atoi(argv[3]);
    bool save = atoi(argv[4]);

    //get other cl args 
    string pathParams = argv[2];
    cout << "Image path: " << pathImage << endl;

    // load parameters from file
    //check if file exists
    CheckFileExists(pathParams);
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

    // IMAGE DECLARATION AND OUT LINES/CIRECLES
    // work on the grey scale image for canny
    Mat greyImg;
    cvtColor(img, greyImg, COLOR_BGR2GRAY);
    Mat edgeMap;
    //lines
    Mat LinesImg = img.clone();
    Mat filledLinesImg = img.clone();
    vector<Vec2f> lines; //Array of 2-elements vectors (ρ,θ)
    //circles
    Mat greyImgBlur;
    vector<Vec3f> circles; //each element is a 3-element floating-point vector (x, y, radius)
    medianBlur(greyImg, greyImgBlur, 3);
    Mat circlesImg = img.clone();
    cout << "\nMedian Blur done" << endl;

    if (tune){
        /**********************************************************
         *                      parameters                        *
         * *******************************************************/

        CannyParams cParams = {static_cast<int>(params["lowThreshold_canny"]), 
                                static_cast<int>(params["highThreshold_canny"]), 
                                static_cast<int>(params["sigma_canny"]), 
                                greyImg, 
                                edgeMap, 
                                WINDOW_EDGES};
        HoughLinesParams hlParams = {static_cast<int>(params["threshold_HoughLines"]), 
                                static_cast<int>(params["minTheta_HoughLines"]), 
                                static_cast<int>(params["maxTheta_HoughLines"]), 
                                edgeMap,
                                LinesImg,
                                lines, 
                                WINDOW_LINES};

        HoughCirclesParams hcParams = {static_cast<int>(params["highThreshold_HoughCircles"]), 
                                    static_cast<int>(params["centerThreshold_HoughCircles"]), 
                                    static_cast<int>(params["minDist_HoughCircles"]),
                                    static_cast<int>(params["minRad_HoughCircles"]), 
                                    static_cast<int>(params["maxRad_HoughCircles"]), 
                                    greyImgBlur,
                                    circlesImg, 
                                    circles,
                                    WINDOW_CIRCLES};
        params_trackbar pTrackbar = {cParams, hlParams, hcParams};

        /**********************************************************
                        EDGE DETECTION 
        ***********************************************************/
        // Generate the edge map with the Canny algorithm. 
        namedWindow(WINDOW_EDGES, WINDOW_AUTOSIZE);  
        imshow(WINDOW_EDGES, greyImg);

        cout << "\nWarning: you can tune the lower and higher thresholds with the trackbars" << endl;
        cout << "While the sigma is fixed in the file, sigma must be between 3 and 7" << endl;
        createTrackbar("lowThreshold", WINDOW_EDGES, 
                        &(pTrackbar.canny_params.lowThreshold), 500, on_trackbar_canny, (void*)&pTrackbar);
        createTrackbar("highThreshold", WINDOW_EDGES, 
                        &(pTrackbar.canny_params.highThreshold), 800, on_trackbar_canny, (void*)&pTrackbar);

        /**********************************************************
         *               HOUGH LINES DETECTION                    *
        **********************************************************/

        // Use the Edge Map as input for the standard Hough transform. 
        namedWindow(WINDOW_LINES, WINDOW_AUTOSIZE);  
        imshow( WINDOW_LINES, LinesImg);

        cout << "\nWarning: you can tune the threshold, minimum and maximum angle" << endl;
        cout << "While the discretization of the grid is fixed by a global variable" << endl;
        createTrackbar("threshold", WINDOW_LINES, 
                        &(pTrackbar.hough_lines_params.threshold), 700, on_trackbar_HLines, (void*)&pTrackbar);
        createTrackbar("minTheta", WINDOW_LINES, 
                        &(pTrackbar.hough_lines_params.minTheta), 4, on_trackbar_HLines, (void*)&pTrackbar);
        createTrackbar("maxTheta", WINDOW_LINES, 
                        &(pTrackbar.hough_lines_params.maxTheta), 4, on_trackbar_HLines, (void*)&pTrackbar);


        /**********************************************************
         *              HOUGH CIRCLES DETECTION                  *
         * *******************************************************/
        namedWindow(WINDOW_CIRCLES, WINDOW_AUTOSIZE);
        imshow(WINDOW_CIRCLES, circlesImg);
        createTrackbar("high threshold", WINDOW_CIRCLES, 
                        &(pTrackbar.hough_circles_params.highThreshold), 800, on_trackbar_HCircles, (void*)&pTrackbar);
        createTrackbar("center threshold", WINDOW_CIRCLES, 
                        &(pTrackbar.hough_circles_params.centerThreshold), 200, on_trackbar_HCircles, (void*)&pTrackbar);
        createTrackbar("min distance", WINDOW_CIRCLES, 
                        &(pTrackbar.hough_circles_params.minDistance), 10, on_trackbar_HCircles, (void*)&pTrackbar);
        createTrackbar("min radius", WINDOW_CIRCLES, 
                        &(pTrackbar.hough_circles_params.minRadius), 10, on_trackbar_HCircles, (void*)&pTrackbar);
        createTrackbar("max radius", WINDOW_CIRCLES, 
                        &(pTrackbar.hough_circles_params.maxRadius), 100, on_trackbar_HCircles, (void*)&pTrackbar);

        waitKey(0);

        /**********************************************************
         *              SPECIFIC PROCESSING                       *
         * *******************************************************/
        lines = pTrackbar.hough_lines_params.out_lines;

        if (fileName == "road2"){road2Img_processing (filledLinesImg, lines);}
        else if (fileName == "road4"){road4Img_processing (filledLinesImg, lines);}
        else if (fileName == "road3"){road3Img_processing (filledLinesImg, lines);}


        // save the image
        DrawInterceptionLines(LinesImg, lines);
        edgeMap = pTrackbar.canny_params.out_image.clone();
        imwrite("results/Lines_"+fileName+".png", LinesImg);
        imwrite("results/edgeMap_"+fileName+".png", edgeMap);
        imwrite("results/FilldLines_"+fileName+".png", filledLinesImg);
        circlesImg = pTrackbar.hough_circles_params.out_image.clone();
        circles = pTrackbar.hough_circles_params.out_circles;
        DrawFilledCircles(filledLinesImg, circles);
        imwrite("results/Circles_"+fileName+".png", filledLinesImg);

        // save parameters
        if (save){
            cout << "Warning: saving parameters:" << endl;
            fstream file;
            file.open("params/params_"+fileName+".txt", ios::out);
            file << "sigma_canny " << pTrackbar.canny_params.sigma << endl;
            file << "lowThreshold_canny " << pTrackbar.canny_params.lowThreshold << endl;
            file << "highThreshold_canny " << pTrackbar.canny_params.highThreshold << endl;
            file << "rho_HoughLines " << 1 << endl;
            file << "theta_HoughLines " << 0.05 << endl;
            file << "threshold_HoughLines " << pTrackbar.hough_lines_params.threshold << endl;
            file << "minTheta_HoughLines " << pTrackbar.hough_lines_params.minTheta << endl;
            file << "maxTheta_HoughLines " << pTrackbar.hough_lines_params.maxTheta << endl;
            file << "highThreshold_HoughCircles " << pTrackbar.hough_circles_params.highThreshold << endl;
            file << "centerThreshold_HoughCircles " << pTrackbar.hough_circles_params.centerThreshold << endl;
            file << "minDist_HoughCircles " << pTrackbar.hough_circles_params.minDistance << endl;
            file << "minRad_HoughCircles " << pTrackbar.hough_circles_params.minRadius << endl;
            file << "maxRad_HoughCircles " << pTrackbar.hough_circles_params.maxRadius << endl;
            file.close();
        }
    }
    else{
        /**********************************************************
         *                         Canny                          *
         * *******************************************************/
        Canny(greyImg, 
            edgeMap, 
            static_cast<int>(params["lowThreshold_canny"]), 
            static_cast<int>(params["highThreshold_canny"]), 
            static_cast<int>(params["sigma_canny"]));
        imshow(WINDOW_EDGES, edgeMap);
        imwrite("results/edgeMap_"+fileName+".png", edgeMap);

        /**********************************************************
         *                         HoughLines                     *
         * *******************************************************/
        HoughLines(edgeMap, lines, 
                        RHO_CELL_SIZE, 
                        THETA_CELL_SIZE, 
                        params["threshold_HoughLines"], 0, 0, 
                        params["minTheta_HoughLines"],
                        params["maxTheta_HoughLines"]);
        cout << "\n\n" << lines.size() << " lines detected" << endl;
        
        DrawInterceptionLines(LinesImg, lines);
        imshow(WINDOW_LINES,LinesImg);
        imwrite("results/Lines_"+fileName+".png", LinesImg);

        // SPECIFIC PROCESSING FOR EACH IMAGE
        if (fileName == "road2"){road2Img_processing (filledLinesImg, lines);}
        else if (fileName == "road4"){road4Img_processing (filledLinesImg, lines);}
        else if (fileName == "road3"){road3Img_processing (filledLinesImg, lines);}

        namedWindow(WINDOW_FILLED_LINES, WINDOW_AUTOSIZE);  
        imshow(WINDOW_FILLED_LINES, filledLinesImg);
        imwrite("results/FilldLines_"+fileName+".png", filledLinesImg);

        /**********************************************************
         *              HOUGH CIRCLES DETECTION                  *
         * *******************************************************/
        // work on the grey scale image
        HoughCircles(greyImgBlur, circles, HOUGH_GRADIENT, 
                        1, //1 = dp = 1
                        (params["minDist_HoughCircles"]), 
                        (params["highThreshold_HoughCircles"]), 
                        (params["centerThreshold_HoughCircles"]), 
                        (params["minRad_HoughCircles"]), 
                        (params["maxRad_HoughCircles"]));
        cout << "\n\n" << circles.size() << " circles detected" << endl;
        DrawFilledCircles(filledLinesImg, circles);
        imshow(WINDOW_CIRCLES, filledLinesImg);
        imwrite("results/Circles_"+fileName+".png", filledLinesImg);
    }

    return 0;
}
