#undef UNICODE

#include <sys/types.h>
#include <sys/socket.h>
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

using namespace std;

void error(string msg)
{
    perror(msg.c_str());
    exit(1);
}

int main(int argc, char *argv[])
{
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
    serv_addr.sin_addr.s_addr = inet_addr("10.10.6.120");

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
