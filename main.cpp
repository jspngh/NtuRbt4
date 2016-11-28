#include "vision.hpp"
#include "robot.hpp"

int main(int, char**)
{
    // 1. Read the camera frames and open a window to show it.

    // 2. Segment the object(s) and calculate the centroid(s) and principle angle(s).

    // 3. Use prespective transform to calculate the desired pose of the arm.

    // 4. Move the arm to the grasping pose by sendCommand() function.
    // The following lines give an example of how to send a command.
    // You can find commends in "Robot Arm Manual.pdf", chap 3, section O-(5)

    run_vision();
    //Robot r;
    //r.grip({1,2});

    return 0;
}
