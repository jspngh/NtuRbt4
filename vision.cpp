#include <vector>

#include "image.hpp"
#include "vision.hpp"

using namespace cv;
using namespace std;

#define NUM_TEMPLATES 3
#define MIN_AREA_THRESHOLD 20
#define MIN_ENCLOSING_AREA_THRESHOLD 1500
#define MIN_DISTANCE_THRESHOLD 30


Point2f get_mass_center(vector<Point> contour)
{
    Moments m = moments(contour, false);
    return Point2f(m.m10/m.m00 , m.m01/m.m00);
}

vector<vector<Point>> filter_objects(vector<vector<Point>> contours)
{
    vector<vector<Point>> filtered;
    for (int i = 0; i< contours.size(); i++)
    {
        // check if area is big enough
        double area=contourArea(contours[i],false);
        if (area < MIN_AREA_THRESHOLD)
            continue;
       
        // check if enclosing rectangle is big enough
        Rect rect = boundingRect(contours[i]);
        area = rect.width * rect.height;
        if (area < MIN_ENCLOSING_AREA_THRESHOLD)
            continue;

        // check if they do not overlap
        bool accepted = true;
        Point2f center = get_mass_center(contours[i]);
        for (int j = 0; j < filtered.size(); j++)
        {
            // calculate euclidian norm between both center of mass
            // and compare with threshold
            if (norm(center - get_mass_center(filtered[j])) < MIN_DISTANCE_THRESHOLD)
            {
                accepted = false;
                break;
            }
        }
        
        if (accepted)
            filtered.push_back(contours[i]);
    }

    return filtered;
}


vector<vector<Point>> find_objects(Mat gray)
{
    Mat canny_output;
    vector<vector<Point>> objects;
    vector<Vec4i> hierarchy;

    Canny(gray, canny_output, 50, 150, 3);
    findContours(canny_output, objects, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    objects = filter_objects(objects);
    
    return objects;
}


Mat crop_image(Mat frame)
{
    int offset_x = 90;
    int offset_y = 50;

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

    vector<vector<Point>> contours = find_objects(img->cvImage);

    // Draw contours
    Mat drawing = Mat::zeros(img->cvImage.size(), CV_8UC3);
    cv::RNG rng(0);
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
        drawContours(drawing, contours, i, color);
        circle(drawing, get_mass_center(contours[i]), 2, color, -1, 8, 0);
    }

    namedWindow("Contours", CV_WINDOW_AUTOSIZE);
    imshow("Contours", drawing);
    waitKey(0);

    delete img;
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
        //find_moments(frame_bw);
    }

}

void run_vision()
{
    //read_templates();

    //for(int i=1; i <= 8; i++)
    //{
        //string path = "res/calib" + to_string(i) + ".png";
        //cout << "processing: " << path << endl;
        //Mat m = imread(path);
        //process_image(m);
    //}

    Mat m = imread("test1.png");
    process_image(m);

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
}
