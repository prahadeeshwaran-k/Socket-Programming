#include <stdio.h>
#include <ifaddrs.h>
#include <arpa/inet.h>

int main()
{
    struct ifaddrs *ifaddr, *ifa;

    if (getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
        return 1;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
            continue;

        if (ifa->ifa_addr->sa_family == AF_INET)
        { // IPv4
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET,
                        &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr,
                        ip, sizeof(ip));
            printf("Interface: %s --> IP: %s\n", ifa->ifa_name, ip);
        }
    }

    freeifaddrs(ifaddr);
    return 0;
}
