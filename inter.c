#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static int exitif = 0;
#define BUFFER 255

int show_prompt(char *prompt);
int readlinen(char *, int);

int main(int argc, char *argv[])
{
    char cmdline[BUFFER];
    char prompt[BUFFER] = "> ";

    char *split;
    char *token;

    while (1)
    {
        show_prompt(prompt);

        if (readlinen(cmdline, BUFFER) == -1)
            break;

        split = cmdline;
        token = strtok_r(split, " ,.", &split);

        printf ("%s - %s\n", token, split);
    }
    return 0;
}

int show_prompt(char * prompt)
{
    printf("%s", prompt);
    return 0;
}

int readlinen(char *str, int len)
{
    int i=0;
    char chr = 0;
    while (1) {
        if (i >= len){
            while ((fgetc(stdin)) != '\n');
            break;
        }
        chr = fgetc(stdin);
        str[i] = chr;
        if (!(chr - '\n')){
            break;
        }
        if (chr == EOF){
            return -1;
        }
        i++;
    }
    str[i] = '\0';
    return i;
}

