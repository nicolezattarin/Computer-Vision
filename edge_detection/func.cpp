#include "func.h"
#include <cmath>
// coun lines in a file

int countLines(const string filename)
{
    std::ifstream in(filename);
    std::string line;
    int lines = 0;
    while (std::getline(in, line))
        ++lines;
    return lines;
}

map<string, double> loadParams(const string filename)
{
    map<string, double> params;
    ifstream infile(filename);
    string line;
    while (getline(infile, line))
    {
        stringstream ss(line);
        string key;
        double value;
        ss >> key >> value;
        params[key] = value;
    }
    return params;
}

vector<Point> PolarToCartesian(float rho, float theta){
    Point pt1, pt2;
    double ct = cos(theta), st = sin(theta); 
    double x0 = ct*rho, y0 = st*rho;
    pt1.x = cvRound(x0 + 1000*(-st));
    pt1.y = cvRound(y0 + 1000*(ct));
    pt2.x = cvRound(x0 - 1000*(-st));
    pt2.y = cvRound(y0 - 1000*(ct));
    vector<Point> cartesianLine = {pt1, pt2};
    return cartesianLine;
}

vector<Vec2f> CartesianToPolar(float x, float y){
    float rho=sqrt(x*x+y*y);
    float theta=atan(y/x);
    vector<Vec2f> point = {rho, theta};
    return point;
}


// gets polar lines from the image
void DrawLines( Mat& img, vector<Vec2f>lines) {
    for( size_t i = 0; i < lines.size(); i++ ){
        float rho = lines[i][0], theta = lines[i][1];
        vector<Point>CartesianLine = PolarToCartesian (rho, theta);
        Point pt1 = CartesianLine[0], pt2 = CartesianLine[1];
        line(img, pt1, pt2, Scalar(0,0,255));
        }
}

// draw lines up to the intersection
void DrawInterceptionLines(Mat& img, vector<Vec2f>lines) {
    vector<Vec2f> IntersectedLines;
    for( size_t i = 0; i < lines.size(); i++ ){
        for (size_t j = i; j< lines.size(); j++){
            vector<Point> line1 = PolarToCartesian(lines[i][0], lines[i][1]);
            vector<Point> line2 = PolarToCartesian(lines[j][0], lines[j][1]);
            Point2f intersection;
            if (Intersection(line1[0], line1[1], line2[0], line2[1], intersection)){
                vector<Point> vertices(3);
                // we choose as vertices the points of the lines with lower y
                if (line1[0].y>line1[1].y){vertices[0] = line1[0];}
                else {vertices[0] = line1[1];}
                if (line2[0].y>line2[1].y){vertices[1] = line2[0];}
                else {vertices[1] = line2[1];}
                vertices[2] = intersection;
                line(img, vertices[0], vertices[2], Scalar(0,0,255));
                line(img, vertices[1], vertices[2], Scalar(0,0,255));
            }
        }
    }
}

bool isIntersecting(Point2f p1, Point2f p2, Point2f q1, Point2f q2) {
    return (((q1.x-p1.x)*(p2.y-p1.y) - (q1.y-p1.y)*(p2.x-p1.x))
            * ((q2.x-p1.x)*(p2.y-p1.y) - (q2.y-p1.y)*(p2.x-p1.x)) < 0)
            &&
           (((p1.x-q1.x)*(q2.y-q1.y) - (p1.y-q1.y)*(q2.x-q1.x))
            * ((p2.x-q1.x)*(q2.y-q1.y) - (p2.y-q1.y)*(q2.x-q1.x)) < 0);
}

// Find intersection of two lines (line1 and line2) or return false
bool Intersection(Point2f p1, Point2f p2, Point2f p3, Point2f p4,  Point2f& intersection) {
    if (isIntersecting(p1, p2,p3, p4)){
        Point2f x = p3 - p1;
        Point2f d1 = p2 - p1;
        Point2f d2 = p4 - p3;
        float cross = d1.x*d2.y - d1.y*d2.x;
        double t1 = (x.x * d2.y - x.y * d2.x)/cross;
        intersection = p1 + d1 * t1;
        return true;
    }
    else{return false;}
}

void road2Img_processing (Mat& img, vector<Vec2f> lines){
    // get all the lines and for all lines that 
    // have intersection, fill the gap between them
    for( size_t i = 0; i < lines.size(); i++ ){
        for (size_t j = i; j< lines.size(); j++){
            vector<Point> line1 = PolarToCartesian(lines[i][0], lines[i][1]);
            vector<Point> line2 = PolarToCartesian(lines[j][0], lines[j][1]);
            Point2f intersection;
            if (Intersection(line1[0], line1[1], line2[0], line2[1], intersection) && intersection.y>165){
                vector<Point> vertices(3);
                // we choose as vertices the points of the lines with lower y
                if (line1[0].y>line1[1].y){vertices[0] = line1[0];}
                else {vertices[0] = line1[1];}
                if (line2[0].y>line2[1].y){vertices[1] = line2[0];}
                else {vertices[1] = line2[1];}
                vertices[2] = intersection;

                fillConvexPoly(img, vertices, Scalar(0,0,255));// gets vertices of polygon to fill
            }
        }
    }
}
