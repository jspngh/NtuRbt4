#include "region.hpp"

Region::Region(int **image, int id, int top, int bottom, int left, int right)
{
    this->image = image;
    this->id = id;
    this->top = top;
    this->bottom = bottom;
    this->left = left;
    this->right = right;

    find_centroid();
    find_principle_angle();
}

void Region::find_centroid()
{
    double A = (double) moment(0, 0);
    double x_c = ((double)moment(1, 0)) / A;
    double y_c = ((double)moment(0, 1)) / A;
    centroid = std::make_pair(x_c, y_c);
}

void Region::find_principle_angle()
{
    double tmp1 = 2.0 * central_moment(1,1);
    double tmp2= central_moment(2,0) - central_moment(0,2);
    double phi = 1.0/2.0 * atan2(tmp1, tmp2);
    principle_angle = phi;
}

int Region::moment(int k, int j)
{
    int sum = 0;
    for (int y = top; y <= bottom; y++)
    {
        for (int x = left; x <= right; x++)
        {
            if (this->image[y][x] == id)
                sum += pow(x, k) * pow(y, j);
        }
    }
    return sum;
}

int Region::central_moment(int k, int j)
{
    int sum = 0;
    for (int y = top; y <= bottom; y++) {
        for (int x = left; x <= right; x++) {
            if (this->image[y][x] == id) {
                sum += pow((x - centroid.first), k) * pow((y - centroid.second), j);
            }
        }
    }
    return sum;
}
