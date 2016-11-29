#undef UNICODE

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <string>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "4000"
#define DEFAULT_INTERFACE "wlp2s0"

using namespace std;

void error(string msg)
{
    perror(msg.c_str());
    exit(1);
}

int main(int argc, char *argv[])
{
    // find interface IP address
    int tmp_sockfd;
    struct ifreq ifr;
    tmp_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    // Get an IPv4 IP address
    ifr.ifr_addr.sa_family = AF_INET;
    // get IP address of the default network interface
    strncpy(ifr.ifr_name, DEFAULT_INTERFACE, IFNAMSIZ-1);
    ioctl(tmp_sockfd, SIOCGIFADDR, &ifr);
    close(tmp_sockfd);
    struct in_addr ip_address = ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr;

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[DEFAULT_BUFLEN];

    portno = atoi(DEFAULT_PORT);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    server = gethostbyname("localhost");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr = ip_address;

    serv_addr.sin_port = htons(portno);

    while (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        printf("%s\n", "Connection refused, retrying...");
        sleep(3);
    }

    int result = 0;
    char sendbuf[] = "MSG RCVD";
    while (result >= 0)
    {
        bzero(buffer, DEFAULT_BUFLEN);
        result = read(sockfd, buffer, DEFAULT_BUFLEN-1);
        if (result < 0)
        {
            error("ERROR reading from socket");
        }
        printf("MSG: %s\n", buffer);

        result = write(sockfd, sendbuf, strlen(sendbuf)+1);
        if (result < 0)
        {
            error("ERROR writing to socket");
        }
    }

    close(sockfd);

    return 0;
}
