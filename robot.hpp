#ifndef M_ROBOT
#define M_ROBOT

#include "server.hpp"

struct RobotCoord
{
    float x;
    float y;
};

class Robot
{
    public:
        Robot();
        ~Robot();

        Server* getServer();
        void grip(RobotCoord c);
        void manual_control();

        RobotCoord img2robot_v(int x_im, int y_im);
        RobotCoord img2robot_l(int x_im, int y_im);
        RobotCoord img2robot_w(int x_im, int y_im);
    private:
        Server* server;
};

#endif
