#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "fatal.h"
#include "hex.h"

#define BUFFER 255

int main(int argc, char *argv[])
{
    if (argc < 2){
        printf("Usage:\n    %s <string>\n", argv[0]);
        exit (-1);
    }

    printf("main pid: %d\n", getpid());

    int pipefd[2];
    char buffer[BUFFER] = {0};
    
    if (pipe(pipefd) == -1){
        fatal("On create pipe.");
    }

    pid_t pid = fork();
    int size = 0;

    if (pid == 0){
        close(pipefd[0]);
        printf("Child rocess: Sending pid to Parent\n");
        printf("Pid of child: %d\n", getpid());
        sprintf(buffer, "%d", getpid());
        write(pipefd[1], buffer, strlen(buffer));
        close(pipefd[1]);
    }

    if (pid > 0){
        close(pipefd[1]);
        printf("Pid of parent: %d\n", getpid());
        printf("Receiving pid from Child\n");
        while ( 1 ){
            (size = read(pipefd[0], buffer, 1));
            printf("size: %d\tbuffer: %s\n", size, buffer);
            if (size <= 0)
                break;
        }
        close(pipefd[0]);
    }

    return 0;
}
