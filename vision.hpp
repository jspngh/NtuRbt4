#ifndef M_VISION
#define M_VISION

#include <vector>
#include <utility>

#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

std::vector<std::pair<cv::Point,double>> get_objects();

#endif
