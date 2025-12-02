// UDP Send Client
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

char message[4096];

int main()
{
    int socketFD;
    struct sockaddr_in server_address;

    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1"); 

    socketFD = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFD < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    printf("Socket SUCCESSFULLY Created, FD: %d\n", socketFD);

    while (1)
    {
        printf("Enter Message: ");
        scanf("%s", message);

        if (sendto(socketFD, message, strlen(message) + 1, 0,
                   (struct sockaddr *)&server_address,
                   sizeof(server_address)) < 0)
        {
            perror("sendto");
            exit(EXIT_FAILURE);
        }
    }
}
