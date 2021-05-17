#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "fatal.h"

void usage(char *prog){
    printf("Usage:\n"
            "\t<%s>\t<Num>\n", prog);
    exit(-1);
}

void byte_memp(char *ptr, size_t size)
{
    if (ptr == NULL)
    {
        fatal("NULL Addr");
    }
    unsigned char * addr;
    printf("Content (char) in [%p, %p] byte-wise\n", ptr, ptr + size -1);
    for (size_t i=0; i<size; i++)
    {
        addr = ((unsigned char *)ptr + i);
        printf("%p\t0x%02x\n", addr, *addr);
    }
    putc('\n', stdout);
}

void int_memp(int *ptr, size_t size)
{
    if (ptr == NULL)
    {
        fatal("NULL Addr");
    }
    unsigned char * addr;
    printf("Content (int) in [%p, %p] byte-wise\n", ptr, ptr + size -1);
    for (size_t i=0; i<size; i++)
    {
        addr = ((unsigned char *)ptr + i * 4);
        printf("%p:\t0x%02x\t0x%02x\t0x%02x\t0x%02x\n", addr, *addr, *(addr +1), *(addr +2), *(addr +3));
    }
    putc('\n', stdout);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        usage(argv[0]);
    }
    int input[4];
    input[0] = atoi(argv[1]);
    input[1] = 1023;
    input[2] = 1024;
    input[3] = 2048;

    byte_memp((char *)&input, sizeof(int) *4);

    int_memp(input, 4);

    return 0;
}
