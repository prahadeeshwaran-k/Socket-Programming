#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>

int main() {
    char hostbuffer[256];
    char *IPbuffer;
    struct hostent *host_entry;
    int hostname;

    // Get the hostname
    hostname = gethostname(hostbuffer, sizeof(hostbuffer));
    if (hostname == -1) {
        perror("gethostname");
        return 1;
    }

    // Get host information
    host_entry = gethostbyname(hostbuffer);
    if (host_entry == NULL) {
        perror("gethostbyname");
        return 1;
    }

    // Convert to IP string
    IPbuffer = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));

    printf("Hostname : %s\n", hostbuffer);
    printf("IP Address: %s\n", IPbuffer);

    return 0;
}
