#include "filter.h"
using namespace std;
/*----------------------------------------------------------------------------*/
/*                               BASE CLASS                                   */
/*----------------------------------------------------------------------------*/

// constructor filter
filter::filter(cv::Mat img, int size) {
		_in_image = img;
		if (size%2 == 0) size++;
		_size = size;
	}

// filtering function in the base class does nothing, 
// only returns a copy of the input image
void filter::filtering() { _out_image = _in_image.clone();}

//setters and getters
void filter::setSize(int size) {
    if (size%2 == 0) size++;
    _size = size;
    }
    
int filter::getSize() {return _size;}
void filter::setInImage(cv::Mat img) {_in_image = img;}
cv::Mat filter::getInImage() {return _in_image;}
cv::Mat filter::getOutImage() {return _out_image;}

/*----------------------------------------------------------------------------*/
/*                               GAUSSIAN FILTER                              */
/*----------------------------------------------------------------------------*/

void GaussianFilter::filtering(){
    cv::GaussianBlur(_in_image, _out_image, cv::Size(_size, _size), _sigma, _sigma);
}

/*----------------------------------------------------------------------------*/
/*                               MEDIAN FILTER                                */
/*----------------------------------------------------------------------------*/
void MedianFilter::filtering(){
    cv::medianBlur(_in_image, _out_image, _size);
}

/*----------------------------------------------------------------------------*/
/*                               BILATERAL FILTER                             */
/*----------------------------------------------------------------------------*/
void BilateralFilter::filtering(){
    cv::bilateralFilter(_in_image, _out_image, _size, _pixel_diameter, _sigma_range, _sigma_space);
}
