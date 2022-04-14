#include "func.h"

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

void PolarToCartesian(vector<Vec2f> &polarLines, vector<Line> &cartLines) {
    for ( size_t i = 0; i < polarLines.size(); i++ ) {
        float rho = polarLines[i][0], theta = polarLines[i][1];
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        Line line;
        line.a.x = cvRound(x0 + 1000*(-b));
        line.a.y = cvRound(y0 + 1000*(a));
        line.b.x = cvRound(x0 - 1000*(-b));
        line.b.y = cvRound(y0 - 1000*(a));
        cartLines.push_back(line);
    }
}

void DrawLines(string WindowName, Mat& img, vector<Vec2f>lines) {
    for( size_t i = 0; i < lines.size(); i++ ){
        float rho = lines[i][0], theta = lines[i][1]; 
        Point pt1, pt2;
        double ct = cos(theta), st = sin(theta); 
        double x0 = ct*rho, y0 = st*rho;
        pt1.x = cvRound(x0 + 1000*(-st));
        pt1.y = cvRound(y0 + 1000*(ct));
        pt2.x = cvRound(x0 - 1000*(-st));
        pt2.y = cvRound(y0 - 1000*(ct));
        line(img, pt1, pt2, Scalar(0,0,255));
        }
    imshow(WindowName, img);

    }


