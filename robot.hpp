#ifndef M_ROBOT
#define M_ROBOT

#include "server.hpp"

struct RobotCoord
{
    float x;
    float y;
    float z;
};

class Robot
{
    public:
        Robot();
        ~Robot();

        Server* getServer();
        RobotCoord getCurrentPos();
        void setSpeed(int v);
        void grip(RobotCoord c);
        void goHome();
        void move2side();
        void manualControl();

        RobotCoord img2robot_v(int x_im, int y_im);
        RobotCoord img2robot_l(int x_im, int y_im);
        RobotCoord img2robot_w(int x_im, int y_im);
    private:
        Server* server;

        int base_height = 252;
        int l1_lengt = 270;
        int l2_length = 295;
        int tooltip_length = 102;
        int gripper_height = 80; // approximated

        int part_height = 40;
        int grip_height = 20;
        int picture_x_pos = -140;

        void wait_pos_reached(RobotCoord pos);
};

#endif
