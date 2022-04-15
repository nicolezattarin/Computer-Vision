#ifndef UTILS_H
#define UTILS_H

#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include <map>
using namespace std;
using namespace cv;

struct Line{ // a line is represented by two points a and b
    Point a, b;
};

/*************************************************************
*                  UTILS TO LOAD PARAMS                      *
**************************************************************/

void CheckFileExists(string);
int countLines(const string );
map<string, double> loadParams(const string);

/*************************************************************
*                  POLAR <-> CARTESIAN                       *
**************************************************************/ 

// gets rho and theta of a point, returns the corresponding cartesian point
vector<Point> PolarToCartesian(float, float); 
// gets x and y of a point, returns vector<Vec2f> point with {rho, theta}
vector<Vec2f> CartesianToPolar(float x, float y); 

/*************************************************************
*                    DRAW ON IMAGES                          *
**************************************************************/ 

// gets vector<Vec2f> of all the lines (polar system) and draws them on the image
void DrawLines( Mat&, vector<Vec2f>); 
// gets vector<Vec3f> of all the circles (cartesian system) and draws them on the image
void DrawCircles(Mat&, vector<Vec3f>); 
// gets vector<Vec3f> of all the circles(cartesian system) and draws them filled on the image
void DrawFilledCircles(Mat&, vector<Vec3f>); 
// gets vector<Vec2f> of all the lines (polar system) and draws them only up to their interception 
void DrawInterceptionLines(Mat& img, vector<Vec2f>); 

// gets four points defining two lines, returns true if the intersection exists and sets the intersection point
bool Intersection(Point2f, Point2f, Point2f, Point2f, Point2f& );
// gets two points defining a line, returns true if the intersection exists
bool isIntersecting(Point&, Point&, Point&, Point&);

void SortLinesBy (vector<Vec2f>& lines, int sortBy=1, bool ascending=false);

#endif