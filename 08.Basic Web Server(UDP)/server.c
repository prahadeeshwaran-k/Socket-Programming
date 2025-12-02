// udp server
#include <arpa/inet.h>  //for inet_pton() -> inet presentation to net
#include <netinet/in.h> //for sockaddr_in structure.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h> //for socket fuctions.
#include <unistd.h>

int main(int argc, char *argv[])
{
  int listenFd;
  struct sockaddr_in serveraddr, clientaddr;
  char buffer[128];

  // note: for standard HTTP use port 80
  if (argc < 2)
  {
    printf("Usage: %s <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if ((listenFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("socket");
  }

  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons(8080);
  serveraddr.sin_addr.s_addr = INADDR_ANY;

  if ((bind(listenFd, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) < 0)
  {
    perror("bind error");
    close(listenFd);
    exit(EXIT_FAILURE);
  }
  printf("UDP server listening on port %d\n", 8080);
  socklen_t clientlen = sizeof(clientaddr);

  while (1)
  {
    int rcv_len = recvfrom(
        listenFd,
        buffer,
        sizeof(buffer),
        0,
        (struct sockaddr *)&clientaddr,
        &clientlen);
    if (rcv_len < 0)
    {
      perror("recvfrom");
      exit(10);
    }
    printf("received a packet %s:%d -- message = %s",
           inet_ntoa(clientaddr.sin_addr),
           ntohs(clientaddr.sin_port),
           buffer);
    bzero(buffer,sizeof(buffer));
  }
}
