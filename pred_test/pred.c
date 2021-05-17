#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

int main(int argc, char *argv[])
{
    char *buff;
    size_t size = 0;

    ssize_t nread;
    while ((nread = getline(&buff, &size, stdin)) != -1){
        memDump("Read line", buff, size);
    }

    free(buff);

    return 0;
}
