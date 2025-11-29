// Simple TCP Client.
// Vector class code
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Usage: %s <server_IPaddress> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sockFD;
    struct sockaddr_in server_address;
    char buffer[128];

    // 1. Create socket
    sockFD = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFD < 0) {
        perror("socket");
        return 0;
    }

    printf("Socket created with file descriptor %d\n", sockFD);

    // 2. Fill server address
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family      = AF_INET;
    server_address.sin_port        = htons(atoi(argv[2]));
    server_address.sin_addr.s_addr = inet_addr(argv[1]);

    // 3. Connect to server
    if (connect(sockFD, (struct sockaddr *)&server_address, sizeof(server_address)) == 0) {
        printf("Connect Successfully\n");
    } else {
        perror("connect");
        close(sockFD);
        return 0;
    }

    // 4. Send data in loop
    while (1) {
        printf("Enter Data: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("EOF or input error, exiting...\n");
            break;
        }

        // send without the extra +1 if you don't want to send the '\0'
        ssize_t n = write(sockFD, buffer, strlen(buffer));
        if (n < 0) {
            perror("write");
            break;
        }
    }

    close(sockFD);
    return 0;
}
