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
    fd_set rdset, testset;
    socklen_t len;

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0)
    {
        perror("socket");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(2000);
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

    listen(sfd, 5);

    len = sizeof(client);
    FD_ZERO(&rdset);
    FD_SET(sfd, &rdset);

    while (1)
    {
        testset = rdset;

        select(100, &testset, NULL, NULL, NULL);

        for (i = 0; i < 100; i++)
        {
            if (FD_ISSET(i, &testset))
            {
                if (i == sfd)
                {
                    newsfd = accept(sfd, (struct sockaddr *)&client, &len);
                    if (newsfd >= 0)
                    {
                        printf("New connection\n");
                        FD_SET(newsfd, &rdset);
                    }
                }
                else
                {
                    bzero(buf, 120);
                    if (read(i, buf, 120) > 0)
                    {
                        printf("%s\n", buf);
                    }
                    else
                    {
                        close(i);
                        FD_CLR(i, &rdset);
                    }
                }
            }
        }
    }
    return 0;
}