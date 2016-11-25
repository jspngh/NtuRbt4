#ifndef M_VISION
#define M_VISION

#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#define NUM_TEMPLATES 3

int thresh = 100;
int max_thresh = 255;
cv::RNG rng(0);
int min_size = 15;

void run_vision();

#endif
