#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include "headers.h"

char *trim_whitespace(char *str) {
        char *end;

        // Trim leading space
        while (isspace((unsigned char)*str)) str++;

        // All spaces?
        if (*str == 0)
                return str;

        // Trim trailing space
        end = str + strlen(str) - 1;
        while (end > str && isspace((unsigned char)*end)) end--;

        // Write new null terminator
        *(end + 1) = '\0';

        return str;
}

unsigned long long int count_lines(FILE *fp)
{
        unsigned long long int lines = 0;

        int ch;
        while((ch = fgetc(fp)) != EOF)
        {
                if(ch == '\n')
                {
                        lines += 1;
                }
        }

        fseek(fp, 0, SEEK_SET);
        return lines;
}
