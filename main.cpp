#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <string>
#include <stack>

#include "vision.hpp"
#include "server.hpp"


using namespace cv;
using namespace std;

// TODO: place in macros if necessary

int main(int, char**)
{
    // 1. Read the camera frames and open a window to show it.

    // 2. Segment the object(s) and calculate the centroid(s) and principle angle(s).

    // 3. Use prespective transform to calculate the desired pose of the arm.

    // 4. Move the arm to the grasping pose by sendCommand() function.
    // The following lines give an example of how to send a command.
    // You can find commends in "Robot Arm Manual.pdf", chap 3, section O-(5)

    // run_vision();

    Server svr;
    int cl_sockfd = svr.openSocket();

    char speed[] = "SETLINESPEED 100";
    svr.sendCommand(speed, cl_sockfd);
    sleep(3);

    int c, x, y;

    while(true)
    {
        cout << "0 -  continue\nOtherwise - stop" << endl;
        cin >> c;
        if (c)
            break;
        cout << "Please enter x: " << endl;
        cin >> x;
        cout << "Please enter y: " << endl;
        cin >> y;

        string command = "MOVT " + to_string(x) + to_string(y);
        const char* movt = command.c_str();
        svr.sendCommand(movt, cl_sockfd);
        sleep(3);
    }

    char command[] = "GOHOME";
    svr.sendCommand(command, cl_sockfd);

    // 5. Control the gripper to grasp the object.
    // The following lines give an example of how to control the gripper.
    char closeGripper[] = "OUTPUT 48 ON";
    svr.sendCommand(closeGripper, cl_sockfd);
    sleep(1);
    char openGripper[] = "OUTPUT 48 OFF";
    svr.sendCommand(openGripper, cl_sockfd);

    //========== Add your code above ==========//

    pause();

    // shutdown the connection since we're done
    int result = shutdown(cl_sockfd, SHUT_WR);
    if (result == -1) {
        close(cl_sockfd);
        printf("shutdown failed with error: %d\n", errno);
        return 1;
    }

    close(cl_sockfd);

    return 0;
}
