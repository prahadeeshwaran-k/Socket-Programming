#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

char buffer[256];

int main(int argc, char *argv[]) {
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
    return 0;
  }
  printf("Socket Successfully created, FD:%d \n", sockFD);

  // 2. Prepare server address
  serverADDR.sin_family = AF_INET;
  serverADDR.sin_port = htons(atoi(argv[1]));
  serverADDR.sin_addr.s_addr = inet_addr("0.0.0.0");

  // 3. Bind
  if (bind(sockFD, (struct sockaddr *)&serverADDR, sizeof(serverADDR)) == 0)
    printf("Bind Successfully\n");
  else {
    perror("bind");
    return 0;
  }

  // 4. Listen
  if (listen(sockFD, 1) < 0) {
    perror("listen");
    close(sockFD);
    exit(EXIT_FAILURE);
  }
  printf("Waiting for Connection\n");
  socklen_t clientLen = sizeof(clientADDR);

  // 5. Accept a client
  newFD = accept(sockFD, (struct sockaddr *)&clientADDR, &clientLen);
  if (newFD < 0) {
    perror("accept");
    close(sockFD);
    exit(EXIT_FAILURE);
  }
  printf("Client connected, FD: %d\n", newFD);

  // 6. Read in a loop from this client
  while (1) {
    bzero(buffer, sizeof(buffer));
    if (read(newFD, buffer, sizeof(buffer)) == 0) {
      printf("client %d terminated \n", newFD - sockFD);
      exit(0);
    }
    printf("received string: %s \n", buffer);
  }
}
