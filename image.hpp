#ifndef M_IMAGE
#define M_IMAGE

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

#include "region.hpp"

#define SEGMENTATION_THRESHOLD 200
#define REGION_THRESHOLD 10

using namespace cv;
using namespace std;

class Image
{
public:
    Mat cvImage;
    Mat cvImage_result;
    int** mImage;
    int nr_regions;
    Region** regions;
    bool reading_error;

    Image(string file_loc);
    Image(Mat image);
    ~Image();

    void thresholding();
    void segmentation();
    void find_regions();

    void print_debug_info();
    void display_region_metadata();
    void print_region_metadata();
    Mat get_cvImage_result();
private:
    // region growing algorithm
    bool grow_region(int k, int j, int i);
};

#endif
