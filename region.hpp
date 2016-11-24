#ifndef M_REGION
#define M_REGION

#include <math.h>
#include <iostream>
#include <sstream>
#include <stack>

class Region
{
public:
    int **image;

    int id;

    int top; // y coordinate
    int bottom; // y coordinate
    int left; // x coordinate
    int right; // x coordinate

    std::pair<double, double> centroid; // (x_c, y_c)
    double principle_angle; // phi

    Region(int **image, int id, int top, int bottom, int left, int right);
    void find_centroid();
    void find_principle_angle();

    int moment(int k, int j); // moment kj
    int central_moment(int k, int j); // central moment kj
};

#endif
