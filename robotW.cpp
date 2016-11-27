#include "robotW.hpp"

void Robot::img2robot(int x_im, int y_im, int& X, int& Y)
{
   X = x_im * a - b; 
   Y = c - (y_im * a - c);
}
