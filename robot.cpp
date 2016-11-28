#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "robot.hpp"

using namespace std;

Robot::Robot()
{
    server = new Server();
}

Robot::~Robot()
{
    delete server;
}

Server* Robot::getServer()
{
  return server;
}

void Robot::grip(RobotCoord c)
{
    int sockfd;
    if (server->client_sock > -1)
        sockfd = server->client_sock;
    else
        sockfd = server->openSocket();

    char speed[] = "SETLINESPEED 100";
    server->sendCommand(speed, sockfd);
    sleep(3);

    string command = "MOVT " + to_string(c.x) + " " + to_string(c.y);
    const char* movt = command.c_str();
    server->sendCommand(movt, sockfd);
    sleep(3);
}

void Robot::manual_control()
{
    int sockfd;
    if (server->client_sock > -1)
        sockfd = server->client_sock;
    else
        sockfd = server->openSocket();

    char speed[] = "SETLINESPEED 100";
    server->sendCommand(speed, sockfd);
    sleep(3);

    int input, x, y;

    while(true)
    {
        cout << "0 -  continue\nOtherwise - stop" << endl;
        cin >> input;
        if (input)
            break;
        cout << "Please enter x: " << endl;
        cin >> x;
        cout << "Please enter y: " << endl;
        cin >> y;

        string command = "MOVT " + to_string(x) + " " + to_string(y);
        const char* movt = command.c_str();
        server->sendCommand(movt, sockfd);
        sleep(3);
    }

    char command[] = "GOHOME";
    server->sendCommand(command, sockfd);

    char closeGripper[] = "OUTPUT 48 ON";
    server->sendCommand(closeGripper, sockfd);
    sleep(1);

    char openGripper[] = "OUTPUT 48 OFF";
    server->sendCommand(openGripper, sockfd);
    sleep(5);
}

RobotCoord Robot::img2robot_v(int x_im, int y_im)
{
    float a1 = -479.945160192;
    float a2 = 1.18393057953;
    float a3 = 0.00608376171349;

    float b1 = 784.002566698;
    float b2 = 0.0859078821404;
    float b3 = -1.14523556393;

    RobotCoord result;
    result.x = a1 + a2 * x_im + a3 * y_im;
    result.y = b1 + b2 * x_im + b3 * y_im;
    return result;
}

RobotCoord Robot::img2robot_l(int x_im, int y_im)
{
	float a1 = -461.863;
	float a2 = 1.10843;
	float a3 = 0.08355;

	float b1 = 680.783;
	float b2 = 0.0468;
	float b3 = -1.0868;

    RobotCoord result;
    result.x = a1 + a2 * x_im + a3 * y_im;
    result.y = b1 + b2 * x_im + b3 * y_im;
    return result;
}

RobotCoord Robot::img2robot_w(int x_im, int y_im)
{
	float a = 1.128;
	float b = 456.8;
	float c = 352.2;

    RobotCoord result;
    result.x = x_im * a - b;
    result.y = c - (y_im * a - c);
    return result;
}
