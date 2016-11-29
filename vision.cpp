
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
    dilate(canny_output, canny_output, Mat());
    erode(canny_output, canny_output, Mat());
    findContours(canny_output, objects, hierarchy, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    objects = filter_objects(objects);

    return objects;
}


Mat crop_image(Mat frame)
{
    int offset_x = 100;
    int offset_y = 40;

    int width = frame.cols;
    int height = frame.rows;

    cv::Rect roi;
    roi.x = offset_x;
    roi.y = offset_y;
    roi.width = width - offset_x;
    roi.height = height - offset_y;

    return frame(roi);
}


vector< pair<Point,double> >  process_image(Mat frame)
{
    frame = crop_image(frame);
    cvtColor(frame, frame, COLOR_BGR2GRAY);

    vector<vector<Point>> contours = find_objects(frame);

    // Make the image binary
    Mat drawing = Mat::zeros(frame.size(), CV_8UC3);
    cv::RNG rng(0);
    for( int i = 0; i< contours.size(); i++ )
    {
        //matching_template(contours[i]);
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
        Scalar color_black = Scalar(255,255,255);
        drawContours(drawing, contours, i, color_black, CV_FILLED);
    }
    
    erode(drawing, drawing, Mat());
    dilate(drawing, drawing, Mat());

    Image *processed_img = new Image(drawing);
    processed_img->thresholding();
    processed_img->segmentation();
    processed_img->find_regions();

    processed_img->print_region_metadata();

    // visualization of the result
    //processed_img->display_region_metadata();

    return processed_img->get_region_metadata();
}


vector<pair<Point,double>> get_objects()
{
    vector<pair<Point,double>> result;
    //process_templates();

    //for(int i=1; i <= 10; i++)
    //{
        //string path = "res/calib2_" + to_string(i) + ".png";
        //cout << "processing: " << path << endl;
        //Mat m = imread(path);
        //cout << process_image(m).size() << endl;
    //}

    Mat m = imread("test1.png");
    //process_image(m);

    //VideoCapture cap(0);
    //if(!cap.isOpened()) 
    //{
        //cout << "Could not open external webcam." << endl;
        //return result;
    //}

    //Mat frame;
    //cap >> frame; 
    cv::namedWindow("Image used for processing");
    imshow("Image used for processing", m);
    waitKey(0);
    cv::destroyWindow("Image used for processing");
    return process_image(m);
}
