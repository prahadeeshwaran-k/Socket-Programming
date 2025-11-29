#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

char buffer[256];

int main(int argc, char *argv[])
{
    int sockFD, newFD;
    struct sockaddr_in serverADDR, clientADDR;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // 1. Create socket
    sockFD = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFD < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    printf("Socket successfully created, FD: %d\n", sockFD);

    memset(&serverADDR, 0, sizeof(serverADDR));
    serverADDR.sin_family = AF_INET;
    serverADDR.sin_port = htons(atoi(argv[1]));
    serverADDR.sin_addr.s_addr = INADDR_ANY;

    // 3. Bind
    if (bind(sockFD, (struct sockaddr *)&serverADDR, sizeof(serverADDR)) < 0) {
        perror("bind");
        close(sockFD);
        exit(EXIT_FAILURE);
    }
    printf("Bind successfully\n");

    // 4. Listen
    if (listen(sockFD, 5) < 0) {
        perror("listen");
        close(sockFD);
        exit(EXIT_FAILURE);
    }
    printf("Waiting for connection...\n");

    socklen_t clientLen = sizeof(clientADDR);

    while (1) {
        newFD = accept(sockFD, (struct sockaddr *)&clientADDR, &clientLen);
        if (newFD < 0) {
            perror("accept");
            continue;
        }

        printf("New connection, FD: %d\n", newFD);

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            close(newFD);
            continue;
        }

        if (pid == 0) {
            // Child
            close(sockFD); // child doesn't need listening socket

            while (1) {
                memset(buffer, 0, sizeof(buffer));
                ssize_t n = read(newFD, buffer, sizeof(buffer) - 1);
                if (n <= 0) {
                    printf("Client (fd=%d) disconnected\n", newFD);
                    close(newFD);
                    exit(0);
                }

                // Remove newline if present
                buffer[strcspn(buffer, "\r\n")] = '\0';

                printf("Client (fd=%d): %s\n", newFD, buffer);

                if (strcmp(buffer, "bye") == 0) {
                    printf("Client (fd=%d) said bye, closing\n", newFD);
                    close(newFD);
                    exit(0);
                }
            }
        } else {
            // Parent
            close(newFD); // parent doesn't handle this client
        }
    }

    close(sockFD);
    return 0;
}
