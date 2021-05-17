#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

void memDump(char *desc, void *addr, int len) {
    int i;
    unsigned char buff[17];
    unsigned char *ptr = (unsigned char*)addr;

    if (desc != NULL)
        printf ("%s:\n", desc);
    for (i = 0; i < len; i++) {
        if ((i % 16) == 0) {
            if (i != 0)
                printf(" | %s\n", buff);
            printf("  %04x |", i);
        }
        printf(" %02x", ptr[i]);
        if ((ptr[i] < 0x20) || (ptr[i] > 0x7e)) {
            buff[i % 16] = '.';
        } else {
            buff[i % 16] = ptr[i];
        }
        buff[(i % 16) + 1] = '\0';
    }
    while ((i % 16) != 0) {
        printf("   ");
        i++;
    }
    printf(" | %s\n", buff);
}

void signal_handler(int signal)
{
    /* Display a message indicating we have received a signal */
    if (signal == SIGINT) {
        printf("\nSniff interrupted!\n");
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    int i, recv_len, sockfd;
    u_char buffer[9000];

    signal(SIGINT, signal_handler);

    if ((sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_TCP)) == -1){
        printf("  [Error]: In socket\n");
        return -1;
    }

    while(1) {
        recv_len = recv(sockfd, buffer, 8000, 0);
        printf("Got a %d byte packet\n", recv_len);
        memDump("packet", buffer, recv_len);
    }

    return 0;
}
