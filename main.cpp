#include <iostream>

#include "vision.hpp"
#include "robot.hpp"

using namespace std;
using namespace cv;


void stack_objects()
{
    Robot r;

    // move robot arm out of the way of the camera
    r.move2side();
    
    // run vision
    vector<Object> objects = get_objects();
    std::sort(objects.begin(), objects.end());
    std::reverse(objects.begin(),objects.end());

    cout << "number of objects found: " << objects.size() << endl;
    
    auto it = objects.begin();
    RobotCoord stack_center = r.img2robot_v(it->center.x, it->center.y); 
    float stack_angle = it->angle;
    it++;
    int stack_height = r.object_height;

    while (it != objects.end())
    {
        cout << endl << "getting object with area: " << it->area << endl;
        // from image coordinate to robot coordinate
        RobotCoord coord = r.img2robot_v(it->center.x, it->center.y);
        coord.z = r.hover_height + stack_height;

        // go to position above objects
        r.move(coord);

        // orient the gripper correctly
        // TODO 
        
        // go down to grasp the object 
        // grasp the object
        r.lift(r.hover_height + stack_height, it->angle);
        stack_center.z = r.hover_height + stack_height;
        r.move(stack_center);
        r.place(r.grip_height + stack_height, stack_angle);
        stack_height += r.object_height;
        
        // move object to drop zone
        
        it++;
    }

}


void pickup_object()
{
    Robot r;

    // move robot arm out of the way of the camera
    r.move2side();
    
    // run vision
    vector<Object> objects = get_objects();

    cout << "number of objects found: " << objects.size() << endl;
    
    auto it = objects.begin();
    while (it != objects.end())
    {
        // from image coordinate to robot coordinate
        RobotCoord coord = r.img2robot_v(it->center.x, it->center.y); 
    
        // go to position above objects
        r.move(coord);

        // orient the gripper correctly
        // TODO 
        
        // go down to grasp the object 
        // grasp the object
        r.pickAndDrop(it->angle);
        
        // move object to drop zone
        
        it++;
    }
}


int main(int, char**)
{
    //vector<Object> objects = get_objects();
    stack_objects();
    return 0;
}
