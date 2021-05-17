#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    printf("Real USER: %s\n", getenv("USER"));
    printf("Real uid: %d\n", getuid());
    printf("Effective uid: %d\n", geteuid());
    return 0;
}
