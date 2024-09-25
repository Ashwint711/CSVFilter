#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>
#include "headers.h"

unsigned char** get_output_fields(int argc, void *args[])
{
	unsigned char** output_fields = NULL;
	unsigned char* token;
	unsigned char field[50];

	int max_length = 50;
	int k = 0;

	for(int i = 1; i < argc; i++)
	{
		k += 1;
		output_fields = (unsigned char**)realloc(output_fields, sizeof(unsigned char *) * k);
		*(output_fields + (k - 1)) = (unsigned char*) malloc(sizeof(unsigned char) * max_length);

		strcpy(field, args[i]);

		trim_whitespace(field);
		if(!strcmp(field, ",")) continue;
		if(!strcmp(field, "from") || !strcmp(field, "FROM") || !strcmp(field, "From")) break;

		token = strtok(field, ",");

		if(token != NULL)
		{
			strcpy(*(output_fields + (k - 1)), field);

			token = strtok(NULL, ",");
			if(token != NULL)
			{
				k += 1;
				output_fields = (unsigned char**)realloc(output_fields, sizeof(unsigned char *) * k);
				*(output_fields + (k - 1)) = (unsigned char*) malloc(sizeof(unsigned char) * max_length);

				strcpy(*(output_fields + (k - 1)), token);
			}
		}
	}

	*(output_fields + (k - 1)) = NULL;

	return output_fields;
}
/*
int main(int argc, void *args[])
{
	unsigned char **output_fields = get_output_fields(argc, args);

	int i = 0;
	while(output_fields[i] != NULL)
	{
		printf("%s",*(output_fields + i));
		i += 1;
	}

	return 0;
}*/
