#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    pid_t pid = fork();
    if (pid  < 0){
        perror("Failed to fork");
    }

    if (pid == 0)
    {
        printf("Child process executed\n");
    }
    else if (pid > 0) {
        printf("Parent process executed!\n");
        printf("Child Pid: %d\n\n", pid);
    }
    return 0;
}
