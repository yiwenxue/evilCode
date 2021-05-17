#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if(argc < 2){
        printf("Usage:\n  <%s> <addr>\n", argv[0]);
        exit(-1);
    }
    char buffer[255];
    long add =0;
    sscanf(argv[1], "%12x", &add);
    printf("The addr is: 0x%12x\n", add);

    char *ptr;
    ptr = 0x7ffeaca4180a;

    printf("Forbidden: %s", ptr);
    return 0;
}
