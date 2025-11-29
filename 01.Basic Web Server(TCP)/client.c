#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h> //for socket fuctions.
#include <netinet/in.h> //for sockaddr_in structure.
#include <arpa/inet.h>  //for inet_pton() -> inet presentation to net
#include <string.h>

char message[4096];
char buffer[128];

int main(int argc,char* argv[]){
    //note: for standard HTTP use port 80.
    //IP address of google.com -> 142.251.40.174
    if (argc < 3) {
        printf("Usage: %s <server_IPaddress> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int socketFD;
    //note AF_INET = IPv4, AF_INET6 = IPv6
    //datagram SOCK_DGRAM is used to send a single chunk of data.
    struct sockaddr_in server_address;

    bzero(&server_address,sizeof(server_address));
    //make the  all memory to zero.
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(argv[2])); //host to network,short -> both little/big endian format to network supported big endian
    server_address.sin_addr.s_addr = inet_addr(argv[1]);

    /*Convert from presentation format of an Internet number in buffer
    starting at CP to the binary network format and store result for
    interface type AF in buffer starting at BUF.*/
    //inet_pton(AF__INET,ip,sin_addr); // convert string to binary "192.168.1.1" => [192,168,1,1]

    socketFD = socket(AF_INET,SOCK_STREAM,0);
    if (socketFD <0) {
        perror("socket");
        exit(0);
    }
    printf("Socket SUCCESSFULLY Created, FD: %d\n",socketFD);


   if(connect(socketFD,(struct sockaddr*)&server_address,sizeof(server_address)) < 0){
       perror("Connect");
       exit(0);
   }
   printf("Connect Successfully\n");

   //it is get request for server.
   sprintf(message, "GET / HTTP/1.1\r\n\r\n");
   socklen_t bufferlen = strlen(message);

    //after here we connected.

    if(write(socketFD,message,strlen(message)) != bufferlen){
        perror("Send Request Failes");
    }
    memset(buffer, 0,sizeof(buffer));


    int n;
    while((n = read(socketFD,buffer,sizeof(buffer)-1))>0){
        printf("%s",buffer);
    }
    if(n<0){
        perror("read error");
    }

    exit(0);
}
