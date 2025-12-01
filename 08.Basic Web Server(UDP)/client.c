#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h> //for socket fuctions.
#include <netinet/in.h> //for sockaddr_in structure.
#include <arpa/inet.h>  //for inet_pton() -> inet presentation to net
#include <string.h>

char message[4096];

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s <server_IPaddress> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int socketFD;
    struct sockaddr_in server_address;

    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(argv[2])); // host to network,short -> both little/big endian format to network supported big endian
    server_address.sin_addr.s_addr = inet_addr(argv[1]);

    socketFD = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFD < 0)
    {
        perror("socket");
        exit(0);
    }
    printf("Socket SUCCESSFULLY Created, FD: %d\n", socketFD);

    while (1)
    {
        printf("Enter Message: ");
        scanf("%s", &message);
        if (sendto(socketFD, message, strlen(message) + 1, 0, &server_address, sizeof(server_address)) < 0)
        {
            perror("upd_socket");
            exit(0);
        }
    }
}
