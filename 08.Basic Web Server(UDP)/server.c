#include <arpa/inet.h>  //for inet_pton() -> inet presentation to net
#include <netinet/in.h> //for sockaddr_in structure.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h> //for socket fuctions.
#include <unistd.h>

int main(int argc, char *argv[]) {
  int listenFd, connfd, n;
  struct sockaddr_in serveraddr;
  char message[4096];
  char buffer[128];

  // note: for standard HTTP use port 80
  if (argc < 2) {
    printf("Usage: %s <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if ((listenFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
  }

  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons(atoi(argv[1]));
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

  if ((bind(listenFd, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) < 0)
    perror("bind error");
  if (listen(listenFd, 50) < 0)
    perror("listen error");

  while (1) {
    struct sockaddr_in clientaddr;
    socklen_t clientaddr_len;

    printf("Waiting for a connection on port %s\n", argv[1]);
    fflush(stdout);

    connfd = accept(listenFd, (struct sockaddr *)NULL, NULL);
    // Making this as NULL will not collect address of collected client
    bzero(buffer, sizeof(buffer));
    while ((n = read(connfd, buffer, sizeof(buffer)))> 0) {
      printf("%s", buffer);
      if (buffer[strlen(buffer) - 1] == '\n')
        break;
      bzero(buffer, sizeof(buffer));
    }
    if(n<0)
        perror("read error");

    strcpy(message, "HTTP/1.0 200 OK \r\n\r\nHello");
    write(connfd, message, sizeof(message));
    close(connfd);
  }
}
