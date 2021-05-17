#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "fatal.h"
#include "hex.h"

int main(int argc, char *argv[])
{
    if (argc < 3){
        printf("Usage:\n    %s <string> <len>\n", argv[0]);
        exit (-1);
    }

    printf("Argv[1]: %s\nSize[1]: %ld\n\n", argv[1], sizeof(argv[1]));
    memp_byte(argv[1], atoi(argv[2]));

    printf("main pid: %d\n", getpid());

    return 0;
}
