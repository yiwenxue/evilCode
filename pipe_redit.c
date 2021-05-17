#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "fatal.h"
#include "hex.h"

#define BUFFER 255

int main(int argc, char *argv[])
{
    printf("main pid: %d\n", getpid());

    int pipefd[2];
    char buffer[BUFFER] = {0};
    
    if (pipe(pipefd) == -1){
        fatal("On create pipe.");
    }

    dup2(STDIN_FILENO, pipefd[1]);

    while ( read(pipefd[0], buffer, 1)){
        printf("buffer: %s\n", buffer);
    }
    close(pipefd[0]);


    return 0;
}
