// Multi thread web server
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

char buffer[256];
void *server_thread(void *p);

int main() {
    int sockFD, newFD;
    struct sockaddr_in serverADDR, clientADDR;

    // 1. Create socket
    sockFD = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFD < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    printf("Socket successfully created, FD: %d\n", sockFD);

    memset(&serverADDR, 0, sizeof(serverADDR));
    serverADDR.sin_family = AF_INET;
    serverADDR.sin_port = htons(8080);
    serverADDR.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0

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

        // allocate and pass client fd to thread
        int *pclient = malloc(sizeof(int));
        if (!pclient) {
            perror("malloc");
            close(newFD);
            continue;
        }
        *pclient = newFD;

        pthread_t tid;
        if (pthread_create(&tid, NULL, server_thread, pclient) != 0) {
            perror("pthread_create");
            close(newFD);
            free(pclient);
            continue;
        }
        pthread_detach(tid);
    }

    close(sockFD);
    return 0;
}

void *server_thread(void *p) {
    int clientFD = *((int *)p);
    free(p);  // free the malloc from main

    printf("New connection, FD: %d\n", clientFD);
    pthread_t tid = pthread_self();

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int n = read(clientFD, buffer, sizeof(buffer) - 1);
        if (n <= 0) {
            printf("Client (fd=%d) disconnected\n", clientFD);
            close(clientFD);
            pthread_exit(NULL);
        }

        buffer[n] = '\0';
        printf("Client (TID=%ld): %s\n", (long)tid, buffer);

        if (strcmp(buffer, "bye") == 0) {
            printf("Client (TID=%ld) said bye, closing\n", (long)tid);
            close(clientFD);
            pthread_exit(NULL);
        }
    }
}
