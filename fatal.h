#ifndef __FATAL__
#define __FATAL__

#include <stdio.h>
#include <string.h>
#define BUFFER 255
void fatal(char *message) {
    char error_msg[BUFFER + 18];

    strcpy(error_msg, "[!!] Fatal Error: ");
    strncat(error_msg, message, BUFFER);
    perror(error_msg);
    exit(-1);
}

void warning(char *message) {
    char error_msg[BUFFER + 20];
    strcpy(error_msg, "[!!] Fatal Warning: ");
    strncat(error_msg, message, BUFFER);
    perror(error_msg);
}

void *ec_malloc(unsigned int size) {
    void *ptr;

    ptr = malloc(size);
    if(ptr == NULL)
        fatal("in ec_malloc() on memory allocation");
    return ptr;
}

void dump(const unsigned char *data_buffer, const unsigned int length){
    unsigned char byte;
    unsigned int i, j;
    for(i=0; i < length; i++) {
        byte = data_buffer[i];
        printf("%02x ", data_buffer[i]); // Display byte in hex.
        if(((i%16)==15) || (i==length-1)) {
            for(j=0; j < 15-(i%16); j++)
                printf(" ");
            printf("| ");
            for(j=(i-(i%16)); j <= i; j++) { // Display printable bytes from line.
                byte = data_buffer[j];
                if((byte > 31) && (byte < 127)) // Outside printable char range
                    printf("%c", byte);
                else
                    printf(".");
            }
            printf("\n"); // End of the dump line (each line is 16 bytes)
        } // End if
    }
}
#endif
