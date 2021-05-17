#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "fatal.h"

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
            printf(" %04x |", i);
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

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage:\n  <%s> <host> <port>\n", argv[0]);
        exit(-1);
    }
	int sockfd, new_sockfd;  // listen on sock_fd, new connection on new_fd
	struct sockaddr_in host_addr, client_addr;	// my address information
	socklen_t sin_size;
	int recv_length=1, yes=1;
	char buffer[1024];

	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		fatal("in socket");

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		fatal("setting socket option SO_REUSEADDR");
	
	host_addr.sin_family = AF_INET;		 // host byte order
	host_addr.sin_addr.s_addr = inet_addr(argv[1]); // automatically fill with my IP
	host_addr.sin_port = htons(atoi(argv[2]));	 // short, network byte order
	memset(&(host_addr.sin_zero), '\0', 8); // zero the rest of the struct
    printf("Host: %s\nPort: %d\n", argv[1], atoi(argv[2]));

	if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1)
		fatal("binding to socket");

	if (listen(sockfd, 5) == -1)
		fatal("listening on socket");

	while(1) {    // Accept loop
		sin_size = sizeof(struct sockaddr_in);
		new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
		if(new_sockfd == -1)
			fatal("accepting connection");
		printf("server: got connection from %s port %d\n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
		send(new_sockfd, "Hello World!\n", 13, 0);
		recv_length = recv(new_sockfd, &buffer, 1024, 0);
		while(recv_length > 0) {
			/* dump(buffer, recv_length); */
			printf("RECV: %d bytes\n", recv_length);
            memDump("buffer", buffer, recv_length);
			recv_length = recv(new_sockfd, &buffer, 1024, 0);
		}
		close(new_sockfd);
	}
	return 0;
}
