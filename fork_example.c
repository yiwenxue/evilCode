#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int i =0;
    fork();
    fork();
    fork();
    printf("Fuck u man %d.\n", i++);

    return 0;
}
