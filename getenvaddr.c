#include <stdio.h>
#include <stdlib.h>
#include "fatal.h"

void memDump(char *desc, void *addr, int len);

int main(int argc, char *argv[])
{
    if (argc < 3){
        printf("Usage:\n  <%s> <ENV> <PROG>\n", argv[0]);
        return -1;
    }
    unsigned char *ptr = getenv(argv[1]);
    int len = 64;
    memDump(argv[1], ptr, len);

    printf("%s is at: %p\n", argv[1], ptr);

    return 0;
}

void memDump(char *desc, void *addr, int len) {
    int i;
    unsigned char buff[17];
    unsigned char *ptr = (unsigned char*)addr;

    if (desc != NULL)
        printf ("%s:\n", desc);
    for (i = 0; i < len; i++) {
        if ((i % 16) == 0) {
            if (i != 0)
                printf(" | %s\n", buff);
            printf("  %04x |", i);
        }
        printf(" %02x", ptr[i]);
        if ((ptr[i] < 0x20) || (ptr[i] > 0x7e)) {
            buff[i % 16] = '.';
        } else {
            buff[i % 16] = ptr[i];
        }
        buff[(i % 16) + 1] = '\0';
    }
    while ((i % 16) != 0) {
        printf("   ");
        i++;
    }
    printf(" | %s\n", buff);
}
