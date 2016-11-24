#include "robot.hpp"

void Robot::img2robot(int x_im, int y_im, int& X, int& Y)
{
   X = a1 + a2 * x_im + a3 * y_im; 
   Y = b1 + b2 * x_im + b3 * y_im; 
}
