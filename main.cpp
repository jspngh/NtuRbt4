#include <iostream>

#include "vision.hpp"
#include "robot.hpp"

using namespace std;
using namespace cv;


int main(int, char**)
{
    // 1. Read the camera frames and open a window to show it.

    // 2. Segment the object(s) and calculate the centroid(s) and principle angle(s).

    // 3. Use prespective transform to calculate the desired pose of the arm.

    // 4. Move the arm to the grasping pose by sendCommand() function.
    // The following lines give an example of how to send a command.
    // You can find commends in "Robot Arm Manual.pdf", chap 3, section O-(5)

    Robot r;

    // move robot arm out of the way of the camera
    r.move2side();
    
    // run vision
    vector<pair<Point,double>> objects = get_objects();

    cout << "number of objects found: " << objects.size() << endl;
    
    vector<pair<Point,double>>::iterator it = objects.begin();
    while (it != objects.end())
    {
        // from image coordinate to robot coordinate
        RobotCoord coord = r.img2robot_v(it->first.x, it->first.y); 
    
        // go to position above objects
        r.move(coord);

        // orient the gripper correctly
        // TODO 
        
        // go down to grasp the object 
        // grasp the object
        r.grip();
        
        // move object to drop zone
        
        it++;
    }

    return 0;
}
