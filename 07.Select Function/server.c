#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

int main()
{
    int sfd, newsfd, i;
    struct sockaddr_in server, client;
    char buf[120];

    fd_set rdset, testset; // for the select File Descriptor
    socklen_t len;

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0)
    {
        perror("socket");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("0.0.0.0");

    if (bind(sfd, (struct sockaddr *)&server, sizeof(server)) == 0)
    {
        printf("Bind success\n");
    }
    else
    {
        perror("bind");
        return 1;
    }

    listen(sfd, 5); // how many client can be handle

    len = sizeof(client);
    FD_ZERO(&rdset);     // this makes all 1024 bits to zero in rdset
    FD_SET(sfd, &rdset); // here sfd (file descriptor number) equal to bit position is set to monitor the FD

    while (1)
    {
        testset = rdset;                         // take the copy of rdset(it holds all the FD in a single array with bit positions)
        select(100, &testset, NULL, NULL, NULL); // 100 -> how many file descriptor it should monitor (0 to 99) file descriptor

        for (i = 0; i < 100; i++)
        {
            if (FD_ISSET(i, &testset)) // check weather particular bit is set or not in testset
                break;                 // here it clear all the bit except the bit which send request.
        }

        // i == sfd means i ie equal to server FD , which tells it is a new connection request 
        if (i == sfd)//check FD number = bit position, if fail then it move to else part,
        {            //if failmeans then the request information already existing client.
            printf("New connection\n");
            newsfd = accept(sfd, (struct sockaddr *)&client, &len);
            FD_SET(newsfd, &rdset); // the new connection to the existing rdset bits
        }
        else
        {
            bzero(buf, 120);
            if (read(i, buf, 120) > 0)
                printf("client FD: %d ->%s\n",i, buf);
            else
            {
                close(i);
                FD_CLR(i, &rdset);
            }
        }
    }
    return 0;
}
