#include <iostream>
#include <stack>
#include <string>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <string>

#include "image.hpp"

using namespace cv;
using namespace std;

void process_image(Mat frame)
{
    Image* img = new Image(frame);
    if (img->reading_error)
    {
        cout << "Could not open the image, is the name correct?" << endl;
    }

    img->thresholding();
    img->segmentation();
    img->find_regions();

    img->print_region_metadata();

    // visualization of the result
    img->display_region_metadata();

    // store the resulting image
    //cv::imwrite("./images/er7-3-result.jpg", img->get_cvImage_result());

    delete img; 
}

Mat crop_image(Mat frame)
{
    int offset_x = 80;
    int offset_y = 60;

    cv::Rect roi;
    roi.x = offset_x;
    roi.y = offset_y;
    roi.width = frame.size().width - (offset_x);
    roi.height = frame.size().height - (offset_y);

    /* Crop the original image to the defined ROI */

    cv::Mat crop = frame(roi);
    cv::imshow("crop", crop);
    cv::waitKey(0);

    cv::imwrite("noises_cropped.png", crop);

    return crop;
}

int main(int, char**)
{
    VideoCapture cap(1); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat frame_bw, frame;
    namedWindow("edges",1);
    for(;;)
    {
        cap >> frame; // get a new frame from camera
        cvtColor(frame, frame_bw, COLOR_BGR2GRAY);
        imshow("edges", frame_bw);
        //if(waitKey(30) >= 0) break;
        waitKey(0);
        //Mat crop = crop_image(frame_bw);
        //process_image(crop);
    }
    
    return 0;
}
