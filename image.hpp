#ifndef M_IMAGE
#define M_IMAGE

#include <utility>
#include <stack>
#include <vector>
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

#include "region.hpp"

#define SEGMENTATION_THRESHOLD 200
#define REGION_THRESHOLD 1000

struct Object
{
    cv::Point center;
    double area;
    double angle;
    bool operator < (const Object& other) const
    {
        return (area < other.area);
    }
};


class Image
{
    public:
        cv::Mat cvImage;
        cv::Mat cvImage_result;
        int** mImage;
        int nr_regions;
        Region** regions;
        bool reading_error;

        Image(std::string file_loc);
        Image(cv::Mat image);
        ~Image();

        void thresholding();
        void segmentation();
        void find_regions();

        void print_debug_info();
        void display_region_metadata();
        std::vector<Object> get_region_metadata();
        void print_region_metadata();
        cv::Mat get_cvImage_result();
    private:
        // region growing algorithm
        bool grow_region(int k, int j, int i);
};


#endif
