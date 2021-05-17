#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 2){
        printf("Usage:\n  <%s> <string>\n", argv[0]);
        exit(-1);
    }
    char text[2048];
    static int test_val = -72;

    strncpy(text, argv[1], 2048);

    printf("Proper way:\nStr: %s\n", text);

    printf("Wrong way:\nStr: ");
    printf(text);

    puts("");

    return 0;
}
