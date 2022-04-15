#include "images_processing.h"
#include <cmath>


void FillInBetween(Mat& img, vector<Vec2f> lines, float limx, float limy, int R, int G, int B){
    for(size_t i = 0; i < lines.size(); i++ ){
            for (size_t j = i; j< lines.size(); j++){
                vector<Point> line1 = PolarToCartesian(lines[i][0], lines[i][1]);
                vector<Point> line2 = PolarToCartesian(lines[j][0], lines[j][1]);
                Point2f intersection;
                // first fill only the road in the middle
                if (Intersection(line1[0], line1[1], line2[0], line2[1], intersection) 
                                && intersection.y>limy && intersection.x>limx){
                    vector<Point> vertices(3);
                    // we choose as vertices the points of the lines with lower y
                    if (line1[0].y>line1[1].y){vertices[0] = line1[0];}
                    else {vertices[0] = line1[1];}
                    if (line2[0].y>line2[1].y){vertices[1] = line2[0];}
                    else {vertices[1] = line2[1];}
                    vertices[2] = intersection;
                    fillConvexPoly(img, vertices, Scalar(B,G,R));// gets vertices of polygon to fill
                }
            }
        }
    }

void road2Img_processing (Mat& img, vector<Vec2f> lines){
    // get all the lines and for all lines that 
    // have intersection, fill the gap between them
    FillInBetween(img, lines, 20, 165, 255, 0, 127);
}

void road4Img_processing (Mat& img, vector<Vec2f> lines){
    // get all the lines and for all lines that 
    // have intersection, fill the gap between them
    SortLinesBy (lines, 1, false);
    vector<Vec2f> linesa = {lines.begin() , lines.begin()+ 10};
    FillInBetween(img, linesa, 195, 175, 255,0,127);
    SortLinesBy (lines, 0, true);
    vector<Vec2f> linesb = {lines[0] , linesa[0]};
    FillInBetween(img, linesb, 0, 0, 0, 128, 255);
    // DEBUG 
    // int index = 0;
    // lines = linesa;
    // float rho = lines[index][0], theta = lines[index][1];
    // vector<Point>CartesianLine = PolarToCartesian (rho, theta);
    // Point pt1 = CartesianLine[0], pt2 = CartesianLine[1];
    // line(img, pt1, pt2, Scalar(0,255,0), 2);
}

void road3Img_processing (Mat& img, vector<Vec2f> lines){
    // get all the lines and for all lines that 
    // have intersection, fill the gap between them
    SortLinesBy (lines, 1, false);
    vector<Vec2f> linesa = {lines[0] , lines[23]};
    FillInBetween(img, linesa, 195, 175, 255,0,127);
    SortLinesBy (lines, 0, true);

    // DEBUG 
    // int index = 23;
    // float rho = lines[index][0], theta = lines[index][1];
    // vector<Point>CartesianLine = PolarToCartesian (rho, theta);
    // Point pt1 = CartesianLine[0], pt2 = CartesianLine[1];
    // line(img, pt1, pt2, Scalar(0,255,0), 2);
}


