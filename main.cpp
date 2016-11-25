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
#include "robot.hpp"

#define NUM_TEMPLATES 3

using namespace cv;
using namespace std;

// TODO: place in macros if necessary
int thresh = 100;
int max_thresh = 255;
RNG rng(0);
int min_size = 15;


void filter_contours(vector<vector<Point>> contours, int min_area, int min_distance = 10)
{
    for( int i = 0; i< contours.size(); i++ )
    {
        double area=contourArea( contours[i],false);
        if(area<min_area)
            contours.erase(contours.begin() + i);
    }
}


vector<vector<Point>> get_contours(Mat gray)
{
    Mat canny_output;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
 
    /// Detect edges using canny
    Canny( gray, canny_output, 50, 150, 3 );
    /// Find contours
    findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    return contours;
}

void find_moments( Mat gray )
{
    vector<vector<Point> > contours = get_contours(gray);

    /// Get the moments
    vector<Moments> mu(contours.size() );
    for( int i = 0; i < contours.size(); i++ )
    { mu[i] = moments( contours[i], false ); }
 
    ///  Get the mass centers:
    vector<Point2f> mc( contours.size() );
    for( int i = 0; i < contours.size(); i++ )
    { mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }
 
    /// Draw contours
    Mat drawing = Mat::zeros( gray.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, contours, i, color);
        circle( drawing, mc[i], 4, color, -1, 8, 0 );
    }
 
    /// Show in a window
    namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
    imshow( "Contours", drawing );
    waitKey(0);
 
    /// Calculate the area with the moments 00 and compare with the result of the OpenCV function
    printf("\t Info: Area and Contour Length \n");
    for( int i = 0; i< contours.size(); i++ )
        printf(" * Contour[%d] - Area (M_00) = %.2f - Area OpenCV: %.2f - Length: %.2f \n", i, mu[i].m00, contourArea(contours[i]), arcLength( contours[i], true ) );
 
}

void read_templates()
{
    string path = "templates/";
    for (int i=1; i<=NUM_TEMPLATES; i++)
    {
        string templ_file = path + "template" + to_string(i) + ".png";
        Mat templ_im = imread(templ_file);
        Mat frame_bw;
        cvtColor(templ_im, frame_bw, COLOR_BGR2GRAY);
        find_moments(frame_bw);
    }

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

    filter_contours(contours, min_size);

    /// Draw contours
    Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar(255,0,0);
        drawContours( drawing, contours, i, color);
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
    //read_templates();

    //for(int i=1; i <= 8; i++)
    //{
        //string path = "res/calib" + to_string(i) + ".png";
        //cout << "processing: " << path << endl;
        //Mat m = imread(path);
        //process_image(m);
    //}
    
    int x, y;
    Robot *r = new Robot();
    r->img2robot(575,215,x,y);
    cout << x << endl;
    cout << y << endl;


    VideoCapture cap(1); 
    if(!cap.isOpened()) return -1;

    Mat frame_bw, frame;
    namedWindow("edges",1);
    while(1)
    {
        cap >> frame; // get a new frame from camera
        //cvtColor(frame, frame_bw, COLOR_BGR2GRAY);
        imshow("edges", frame);
        waitKey(0);
        //Mat crop = crop_image(frame_bw);
        //process_image(crop);
    }
    
    return 0;
}
