#include "images_processing.h"
#include <cmath>

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

