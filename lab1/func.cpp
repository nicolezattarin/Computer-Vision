#include "func.h"


// This function is automatically called when a mouse event happens
// x,y: coordinates of mouse position, event: type of event, flags: get buttons status
void click(int event, int x, int y, int flags, void *userdata){
    pixel *px = (pixel *)userdata;
    if (event == EVENT_LBUTTONDOWN){
        cout << "left bottom pressed at pixel: " << x << ", " << y << endl;
        px->x = x;
        px->y = y;
        px->l_pressed = true;
    }
    else if (event == EVENT_RBUTTONDOWN){
        cout << "right bottom pressed at pixel: " << x << ", " << y << endl;
        px->x = x;
        px->y = y;
        px->r_pressed = true;
    }
}

void mean_color(Mat img, int x, int y, int size, double &mblue, double &mgreen, double &mred){
    // Captures the left click of the mouse and computes the mean RGB color over a
    // 9x9 neighborhood of the clicked point
    int x_start = x - int(size/2.);
    int y_start = y - int(size/2.);
    int x_end = x + int(size/2.);
    int y_end = y + int(size/2.);
    int sum_b = 0;
    int sum_g = 0;
    int sum_r = 0;
    int count = 0;
    for (int i = x_start; i <= x_end; i++) {
        for (int j = y_start; j <= y_end; j++) {
            Vec3b color = img.at<Vec3b>(j, i);
            // cout << "Pixel: " << i << ", " << j << ": " << color[0] << ", " << color[1] << ", " << color[2] << endl;
            sum_b += color[0];
            sum_g += color[1];
            sum_r += color[2];
            count++;
        }
    }
    mblue = sum_b/double(count);
    mgreen = sum_g/double(count);
    mred = sum_r/double(count);
    cout << "Mean BGR color: " << sum_b/double(count) << ", " << sum_g/double(count) << ", " << sum_r/double(count) << endl;

}

void square_subimage (Mat img, int x, int y, int L, bool show = false){
    Rect crop(x-int(L/2), y-int(L/2), L, L);
    cout << "Sub image: " << crop << endl;
    Mat subimg = img(crop);
    cout << "Sub image size: " << subimg.size() << endl;
    if (show){
        namedWindow("Sub image", WINDOW_AUTOSIZE);
        imshow("Sub image", subimg);
        waitKey(0);
        destroyWindow("Sub image");}
}