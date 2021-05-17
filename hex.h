#include "fatal.h"

void memp_byte(void *pointer, size_t size)
{
    char *ptr = (char *) pointer;
    if (ptr == NULL)
    {
        fatal("On print memory");
    }
    unsigned char * addr;
    printf("Content (char) in [%p, %p] byte-wise\n", ptr, ptr + size -1);
    for (size_t i=0; i<size; i+= 4)
    {
        addr = ((unsigned char *)ptr + i);
        printf("%p\t", addr);
        for (int j = 0; j < 4; ++j) {
            addr = ((unsigned char *)ptr + i + j);
            printf("0x%02x\t", *addr);
        }
        puts("");
    }
    putc('\n', stdout);
}
void memp_4byte(void *pointer, size_t size)
{
    int *ptr = (int *) pointer;
    if (ptr == NULL)
    {
        fatal("On print memory");
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

void binary_print(unsigned int value) {
    unsigned int mask = 0xff000000; 
    unsigned int shift = 256*256*256;
    unsigned int byte, byte_iterator, bit_iterator;
    for(byte_iterator=0; byte_iterator < 4; byte_iterator++) {
        byte = (value & mask) / shift;
        printf(" ");
        for(bit_iterator=0; bit_iterator < 8; bit_iterator++) {
            if(byte & 0x80)
                printf("1");
            else
                printf("0");
            byte *= 2;
        }
        mask /= 256;
        shift /= 256;
    }
}
