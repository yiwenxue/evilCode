#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

void handle_shutdown(int sig);
void time_stamp(int fd);

int main(int argc, char *argv[])
{
    signal(SIGTERM, handle_shutdown);
    signal(SIGINT, handle_shutdown);
    while (1) {
        time_stamp(STDOUT_FILENO);
        printf("Hello World!\n");
        usleep(1e5);
    }
    return 0;
}
void time_stamp(int fd){
    time_t now;
    int length;
    struct tm *time_struct;
    char time_bufff[40];

    time(&now);
    time_struct = localtime((const time_t *)&now);
    length = strftime(time_bufff, 40, "%m/%d/%Y %H:%M:%S > ", time_struct);
    write(fd, time_bufff, length);
}
void handle_shutdown(int sig){
    time_stamp(STDOUT_FILENO);
    printf("Shutting down\n");
    exit(-1);
}
