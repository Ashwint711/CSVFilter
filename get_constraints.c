#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "headers.h"
unsigned char** get_constraints(int start, int argc, void *args[]) {

	char str[50];  // Input string
	char key[50];                 // To store the key
	char value[50];               // To store the value
	char *token;

	unsigned char **constraints = NULL;

	int max_length = 50;
	int k = 0;
	for(int i = start; i < argc; i++)
	{
		strcpy(str, args[i]);   
		if(!strcmp(str, ",")) continue;

		k += 2;
		constraints = (unsigned char**)realloc(constraints, sizeof(unsigned char*) * k);
		*(constraints + (k - 2)) = malloc(sizeof(unsigned char) * max_length);		

		trim_whitespace(str);
		// Split the string using '=' as the delimiter
		token = strtok(str, "=");

		// Store and trim the first part (key)
		if (token != NULL) {
			strcpy(key, token);  // Trim spaces and copy the first token to key
			strcpy(*(constraints + (k - 2)), token);

			// Get the next token (value), trim and store it

			*(constraints + (k - 1)) = malloc(sizeof(unsigned char) * max_length);	

			token = strtok(NULL, "=");
			if (token != NULL) {
				strcpy(value, token);  // Trim spaces and copy the second token to value
				strcpy(*(constraints + (k - 1)), token);
			}else
			{
				i += 1;
				strcpy(str, args[i]);
				trim_whitespace(str);

				token = strtok(str, "=");

				if(token != NULL)
				{
					strcpy(value, token);
					strcpy(*(constraints + (k - 1)), token);
				}
				else
				{
					i += 1;
					strcpy(str, args[i]);
					trim_whitespace(str);

					token = strtok(str, "=");
					strcpy(value, token);
					strcpy(*(constraints + (k - 1)), token);
				}
			}
		}

		// Print key and value
		//	printf("Key: '%s'\n", key);
		//	printf("Value: '%s'\n", value);
	}

	constraints = (unsigned char**)realloc(constraints, sizeof(unsigned char*) * (k+1));
	*(constraints + (k)) = NULL;

	return constraints;
}

