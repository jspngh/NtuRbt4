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

int thresh = 100;
int max_thresh = 255;
RNG rng(12345);
int min_size = 15;


void read_templates()
{
    path

}

void canny(Mat src_gray)
{
    Mat canny_output;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    /// Detect edges using canny
    Canny( src_gray, canny_output, thresh, thresh*2, 3 );
    /// Find contours
    findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    /// Draw contours
    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
        float size = contourArea(contours.at(i));
        Scalar color;
        if (size > min_size)
        {
            color = Scalar(255,0,0);
            drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
            cout << size << endl;
        }
    }

    /// Show in a window
    namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
    imshow( "Contours", drawing );
    waitKey(0);
}

Mat crop_image(Mat frame)
{
    int offset_x = 90;
    int offset_y = 130;

    int width = frame.cols;
    int height = frame.rows;

    cv::Rect roi;
    roi.x = offset_x;
    roi.y = offset_y;
    roi.width = width - offset_x;
    roi.height = height - offset_y;

    return frame(roi);
}


void process_image(Mat frame)
{
    frame = crop_image(frame);
    Image* img = new Image(frame);

    canny(img->cvImage);

    //img->thresholding();
    //img->segmentation();
    //img->find_regions();

    //img->print_region_metadata();

    // visualization of the result
    //img->display_region_metadata();

    delete img; 
}


int main(int, char**)
{
    for(int i=1; i <= 8; i++)
    {
        string path = "res/calib" + to_string(i) + ".png";
        cout << "processing: " << path << endl;
        Mat m = imread(path);
        process_image(m);
    }


    //VideoCapture cap(0); 
    //if(!cap.isOpened()) return -1;

    //Mat frame_bw, frame;
    //namedWindow("edges",1);
    //while(1)
    //{
        //cap >> frame; // get a new frame from camera
        //cvtColor(frame, frame_bw, COLOR_BGR2GRAY);
        //imshow("edges", frame_bw);
        //waitKey(1);
        ////Mat crop = crop_image(frame_bw);
        ////process_image(crop);
    //}
    
    return 0;
}
