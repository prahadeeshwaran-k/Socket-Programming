// UDP Server
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main()
{
    int listenFd;
    struct sockaddr_in serveraddr, clientaddr;
    char buffer[128];

    listenFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (listenFd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(8080);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listenFd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    printf("UDP server listening on port %d\n", 8080);

    socklen_t clientlen = sizeof(clientaddr);

    while (1)
    {
        int rcv_len = recvfrom(listenFd, buffer, sizeof(buffer), 0,
                               (struct sockaddr *)&clientaddr, &clientlen);

        if (rcv_len < 0)
        {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }

        printf("received from %s:%d -- message = %s\n",
               inet_ntoa(clientaddr.sin_addr),
               ntohs(clientaddr.sin_port),
               buffer);

        bzero(buffer, sizeof(buffer));
    }
}
