#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>

#include "server.hpp"

using namespace std;

Server::Server()
{
    int tmp_sockfd;
    struct ifreq ifr;

    tmp_sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // Get an IPv4 IP address
    ifr.ifr_addr.sa_family = AF_INET;

    // get IP address of the default network interface
    strncpy(ifr.ifr_name, DEFAULT_INTERFACE, IFNAMSIZ-1);

    ioctl(tmp_sockfd, SIOCGIFADDR, &ifr);

    close(tmp_sockfd);

    ip_address = ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr;
    printf("Found IP address: %s\n", inet_ntoa(ip_address));

    client_sock = -1;
}

int Server::openSocket()
{
    int svr_sockfd, cl_sockfd;
    socklen_t cl_len;
    struct sockaddr_in svr_addr, cl_addr;

    svr_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (svr_sockfd < 0)
        error("ERROR opening socket");

    bzero((char *) &svr_addr, sizeof(svr_addr));
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_addr = ip_address;
    svr_addr.sin_port = htons(DEFAULT_PORT);

    if (bind(svr_sockfd,
             (struct sockaddr *) &svr_addr,
             sizeof(svr_addr)) < 0)
        error("ERROR on binding");

    listen(svr_sockfd,5);
    cl_len = sizeof(cl_addr);
    cl_sockfd = accept(svr_sockfd,
                       (struct sockaddr *) &cl_addr,
                       &cl_len);
    if (cl_sockfd < 0)
        error("ERROR on accept");

    close(svr_sockfd);

    client_sock = cl_sockfd;
    return cl_sockfd;
}

int Server::sendCommand(char* sendbuf, int client_sock)
{
    cout << "Sending Command: " << sendbuf << endl;
    int snd_result = 0;
    int rcv_result = 0;
    char rcvbuf[DEFAULT_BUFLEN];
    int counter = 0;
    int RTT_threshold = 1000;

    // send command to robot arm
    snd_result = write(client_sock, sendbuf, strlen(sendbuf)+1);
    if (snd_result < 0)
    {
        error("ERROR writing to socket");
    }

    sleep(1);

    do
    {
        rcv_result = read(client_sock, rcvbuf, DEFAULT_BUFLEN-1);
        cout << "In the do while loop" << endl;
        counter++;
    }while(rcv_result == 0 && counter < RTT_threshold);

    if(counter >= RTT_threshold)
    {
        error("Respond timeout");
    }
    else
    {
        if(!strcmp(rcvbuf,"ERR"))
        {
            error("Invalid command");
        }
    }

    printf("Here is the message: %s\n", rcvbuf);

    return 0;
}

void Server::error(string msg)
{
    perror(msg.c_str());
    exit(1);
}
