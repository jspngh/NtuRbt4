#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <string>
#include <vector>

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

RobotCoord Robot::getCurrentPos()
{
    int sockfd;
    if (server->client_sock > -1)
        sockfd = server->client_sock;
    else
        sockfd = server->openSocket();

    string command = "GETPOS X Y Z A B C";
    const char* speed = command.c_str();
    server->sendCommand(speed, sockfd);
    string response = server->getLastResponse();
    RobotCoord result;
    if (response.compare("ERR") == 0)
    {
        printf("%s\n", "GETPOS returned error");
        return result;
    }

    int x_pos = response.find(' ');
    result.x = stoi(response.substr(0, x_pos));
    int y_pos = response.find(' ', x_pos + 1);
    result.y = stoi(response.substr(x_pos + 1, y_pos));
    int z_pos = response.find(' ', y_pos + 1);
    result.z = stoi(response.substr(y_pos + 1, z_pos));
    return result;
}

void Robot::setSpeed(int v)
{
    int sockfd;
    if (server->client_sock > -1)
        sockfd = server->client_sock;
    else
        sockfd = server->openSocket();

    string command = "SETLINESPEED " + to_string(v);
    const char* speed = command.c_str();
    server->sendCommand(speed, sockfd);

    // TODO remove hardcoded value
    command = "SETPTPSPEED 8"; // + to_string(v);
    speed = command.c_str();
    server->sendCommand(speed, sockfd);
    //sleep(3);
}

void Robot::move(RobotCoord c)
{
    int sockfd;
    if (server->client_sock > -1)
        sockfd = server->client_sock;
    else
        sockfd = server->openSocket();
        string command = "MOVP " + to_string((int)c.x) + " " + to_string((int)c.y) + " " + to_string((int)c.z) + " # # #";
        const char* movt = command.c_str();
        server->sendCommand(movt, sockfd);
}

void Robot::rotateGripper(double angle)
{
    this->resetJoints();

    angle = angle/3.141592 * 180.0;
    cout << "angle on image: " << angle << endl;
    if(angle < 0)
        angle + 180;

    angle = angle - 90;

    int sockfd;
    if (server->client_sock > -1)
        sockfd = server->client_sock;
    else
        sockfd = server->openSocket();

    if (angle < -90)
        angle = angle + 180;
    else if (angle > 90)
        angle = angle - 180;

    // manually correcting
    //if (angle < 0)
        //angle = angle - 5;
    //else
        //angle = angle - 15;

    cout << "angle for robot j6 " << angle << endl;

    string command = "MOVJ # # # # # " + to_string(angle);
    const char* grip = command.c_str();
    server->sendCommand(grip, sockfd);
}

void Robot::resetJoints()
{
    int sockfd;
    if (server->client_sock > -1)
        sockfd = server->client_sock;
    else
        sockfd = server->openSocket();

    string command = "MOVJ 0 45 -45 0 -90 0";
    const char* grip = command.c_str();
    server->sendCommand(grip, sockfd);
    //sleep(3);
}

void Robot::lift(int transport_height, double angle)
{
    int sockfd;
    if (server->client_sock > -1)
        sockfd = server->client_sock;
    else
        sockfd = server->openSocket();

    //rotateGripper(angle);

    string command = "MOVP # # " + to_string(grip_height) + " # # #";
    const char* move_down = command.c_str();
    server->sendCommand(move_down, sockfd);
    
    command = "OUTPUT 48 ON";
    const char* close_grip = command.c_str();
    server->sendCommand(close_grip, sockfd);

    command = "MOVP # # " + to_string(transport_height) + " # # #";
    const char* move_up = command.c_str();
    server->sendCommand(move_up, sockfd);

}

void Robot::place(int drop_height, double angle)
{
    int sockfd;
    if (server->client_sock > -1)
        sockfd = server->client_sock;
    else
        sockfd = server->openSocket();

    //rotateGripper(angle);

    string command = "MOVP # # " + to_string(drop_height) + " # # #";
    const char* move_down = command.c_str();
    server->sendCommand(move_down, sockfd);

    command = "OUTPUT 48 OFF";
    const char* open_grip = command.c_str();
    server->sendCommand(open_grip, sockfd);

    command = "MOVP # # " + to_string(hover_height) + " # # #";
    const char* move_up = command.c_str();
    server->sendCommand(move_up, sockfd);

}

void Robot::pickAndDrop(double angle)
{
    lift(hover_height, angle);
    place(grip_height, angle);
}

void Robot::goHome()
{
    int sockfd;
    if (server->client_sock > -1)
        sockfd = server->client_sock;
    else
        sockfd = server->openSocket();

    string command = "GOHOME";
    const char* movt = command.c_str();
    server->sendCommand(movt, sockfd);
    sleep(10);
}

void Robot::move2side()
{
    int sockfd;
    if (server->client_sock > -1)
        sockfd = server->client_sock;
    else
        sockfd = server->openSocket();

    string command = "MOVJ " + to_string(j1_picture_pos) + " # # # # #";
    const char* grip = command.c_str();
    server->sendCommand(grip, sockfd);
    sleep(3);
}

void Robot::manualControl()
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

        string command = "MOVP " + to_string(x) + " " + to_string(y) + " # # # #";
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

void Robot::wait_pos_reached(RobotCoord pos)
{
    RobotCoord current_pos = getCurrentPos();
    float sq_dist = pow(pos.x - current_pos.x, 2) +
                    pow(pos.y - current_pos.y, 2) +
                    pow(pos.z - current_pos.z, 2);

    while (sqrt(sq_dist) > 99)
    {
        sleep(2);
        RobotCoord current_pos = getCurrentPos();
        float sq_dist = pow(pos.x - current_pos.x, 2) +
                        pow(pos.y - current_pos.y, 2) +
                        pow(pos.z - current_pos.z, 2);
    }
}

RobotCoord Robot::img2robot_v(int x_im, int y_im)
{
    float a1 = -370;
    float a2 = 1.15;
    float a3 = 0.05;

    float b1 = 680.0;
    float b2 = 0.048;
    float b3 = -1.115;

    // float a1 = -479.945160192;
    // float a2 = 1.18393057953;
    // float a3 = 0.00608376171349;

    // float b1 = 784.002566698;
    // float b2 = 0.0859078821404;
    // float b3 = -1.14523556393;

    RobotCoord result;
    result.x = a1 + a2 * x_im + a3 * y_im;
    result.y = b1 + b2 * x_im + b3 * y_im;
    result.z = hover_height;
    result.a = -1;
    result.b = -1;
    result.c = -1;
    return result;
}

RobotCoord Robot::img2robot_l(int x_im, int y_im)
{
    float a1 = -351.863;
    float a2 = 1.10843;
    float a3 = 0.08355;

    float b1 = 680.783;
    float b2 = 0.0468;
    float b3 = -1.0868;

    RobotCoord result;
    result.x = a1 + a2 * x_im + a3 * y_im;
    result.y = b1 + b2 * x_im + b3 * y_im;
    result.z = hover_height;
    result.a = -1;
    result.b = -1;
    result.c = -1;
    return result;
}

RobotCoord Robot::img2robot_w(int x_im, int y_im)
{
    float a = 1.128;
    float b = 477.104;
    float c = 363.48;

    RobotCoord result;
    result.x = x_im * a - b;
    result.y = c - (y_im * a - c);
    result.z = hover_height;
    result.a = -1;
    result.b = -1;
    result.c = -1;
    return result;
}
