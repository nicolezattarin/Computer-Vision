#include "func.h"

void showHistogram(std::vector<cv::Mat>& hists, string main_title, bool save)
// plots the histograms of the image
// args:
//      hists = vector of 3 cv::mat of size nbins=256 with the 3 histograms
// e.g.: hists[0] = cv:mat of size 256 with the red histogram
//       hists[1] = cv:mat of size 256 with the green histogram
//       hists[2] = cv:mat of size 256 with the blue histogram

{
  // Min/Max computation
  double hmax[3] = {0,0,0};
  double min;
  cv::minMaxLoc(hists[0], &min, &hmax[0]);
  cv::minMaxLoc(hists[1], &min, &hmax[1]);
  cv::minMaxLoc(hists[2], &min, &hmax[2]);

  std::string wname[3] = { main_title+": blue", main_title+": green", main_title+": red" };
  cv::Scalar colors[3] = { cv::Scalar(255,0,0), cv::Scalar(0,255,0),
                           cv::Scalar(0,0,255) };

  std::vector<cv::Mat> canvas(hists.size());

  // Display each histogram in a canvas
  for (int i = 0, end = hists.size(); i < end; i++)
  {
    canvas[i] = cv::Mat::ones(125, hists[0].rows, CV_8UC3);

    for (int j = 0, rows = canvas[i].rows; j < hists[0].rows-1; j++)
    {
      cv::line(
            canvas[i],
            cv::Point(j, rows),
            cv::Point(j, rows - (hists[i].at<float>(j) * rows/hmax[i])),
            hists.size() == 1 ? cv::Scalar(200,200,200) : colors[i],
            1, 8, 0
            );
    }

    cv::imshow(hists.size() == 1 ? "value" : wname[i], canvas[i]);
    if (save == true){cv::imwrite(hists.size() == 1 ? "value.png" : wname[i]+".png", canvas[i]);}
  }
}
// shows the image
void showImage(cv::Mat& img, string window_name)
{
    namedWindow(window_name, WINDOW_AUTOSIZE);
    imshow(window_name, img);
}

void equalize(cv::Mat& img, cv::Mat& equalized_img, 
            std::vector<cv::Mat>& hists, std::vector<cv::Mat>& equalized_hists, 
            string color_space){

        if (color_space != "RGB" && color_space != "BGR2HSV" && color_space != "BGR2Lab"){
            cout << "color_space must be RGB or HSV" << endl;
            return;}

        //histo params
        int nbins = 256;
        int nimages = 1;
        int dims = 1;
        const int * nchannels = 0;
        float r[] = {0,255};
        const int histSize[] = {nbins};
        const float* ranges[] = {r};
        std::vector<Mat> channels(3);
        Mat hsv;

        if (color_space == "RGB"){
            cout << "RGB equalization" << endl;
            split(img, channels);// split the image into 3 channels
            // equalize each channel
            for (int i = 0; i < 3; i++) equalizeHist(channels[i], channels[i]);
            merge(channels, equalized_img);// merge the equalized channels

            for (int i = 0; i < 3; i++) calcHist(&channels[i], nimages, nchannels, Mat(), equalized_hists[i], 
                                        dims, histSize, ranges, true, false);
        }
        else if (color_space == "BGR2Lab"){
            cout << "BGR2Lab equalization" << endl;
            Mat hsv;
            cvtColor(img, hsv, COLOR_BGR2Lab);
            split(hsv, channels);
            equalizeHist(channels[0], channels[0]); //equalize the luminance channel
            merge(channels, equalized_img);
            cvtColor(equalized_img, equalized_img, COLOR_HSV2BGR); //change back to BGR
            for (int i = 0; i < 3; i++) calcHist(&channels[i], nimages, nchannels, Mat(), equalized_hists[i], 
                                                dims, histSize, ranges, true, false);
        }
        else{
            cout << "BGR2HSV equalization" << endl;
            Mat hsv;
            cvtColor(img, hsv, COLOR_BGR2HSV);
            split(hsv, channels);
            equalizeHist(channels[2], channels[2]); 
            merge(channels, equalized_img);
            cvtColor(equalized_img, equalized_img, COLOR_HSV2BGR); //change back to BGR
            for (int i = 0; i < 3; i++) calcHist(&channels[i], nimages, nchannels, Mat(), equalized_hists[i], 
                                                dims, histSize, ranges, true, false);
        }

        for (int i = 0; i < 3; i++) calcHist(&channels[i], nimages, nchannels, Mat(), hists[i], 
                    dims, histSize, ranges, true, false);
        }

    void saveImage(cv::Mat& img, string path){
        if(img.empty()) cerr << "Imgage is empty." << endl;
        cv::imwrite(path, img); 
    }