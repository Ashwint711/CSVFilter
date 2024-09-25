#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<stdbool.h>
#include "headers.h"


bool validate_constraint_fields(unsigned char** output_fields, FILE *fp)
{
	int result = false;
	unsigned char line[1024];
	char *word;
	char *rest;
	char *subtoken;
	char *sub_rest;

	int i = 0;
	unsigned char colname[50];

	while(output_fields[i] != NULL)
	{
		result = false;

		fseek(fp, 0, SEEK_SET);
		strcpy(colname, output_fields[i]);
		trim_whitespace(colname);

		if(fgets(line, sizeof(line), fp) != NULL)
		{
			char *ln = line;
			word = strtok_r(ln, ",", &ln);

			while(word != NULL)
			{
				trim_whitespace(word);

				subtoken = strtok(word, " ");
				while(subtoken != NULL)	
				{	
					if(strcmp(colname, subtoken) == 0)
					{
						result = true;
						break;
					}


					subtoken = strtok(NULL, " ");
				}	
				word = strtok_r(NULL, ",", &ln);
			}
		}
		i += 2;
		if(result == false)
			break;
	}
	
	return result;
}

/*
   int main()
   {
   FILE *fp;
   fp = fopen("students.csv", "r+");
   if(fp == NULL)
   {
   perror("Failed to open file!\n");
   return -1;
   }
//RollNo,FirstName MiddleName LastName,Gender,Year,Degree

int max_length = 50;
unsigned char **output_fields = (unsigned char **)malloc(sizeof(unsigned char*) * 3);
 *(output_fields + 0) = (unsigned char*) malloc(sizeof(unsigned char) * max_length);
 *(output_fields + 0) = "RollNo";
 *(output_fields + 1) = (unsigned char*) malloc(sizeof(unsigned char) * max_length);
 *(output_fields + 1) = "FirstName";
 *(output_fields + 2) = (unsigned char*) malloc(sizeof(unsigned char) * max_length);
 *(output_fields + 2) = "Gender";
 *(output_fields + 3) = (unsigned char*) malloc(sizeof(unsigned char) * max_length);
 *(output_fields + 3) = "Year";
 *(output_fields + 4) = (unsigned char*) malloc(sizeof(unsigned char) * max_length);
 *(output_fields + 4) = "Degree";
 *(output_fields + 5) = NULL;
 bool res = validate_output_fields(output_fields, fp);
 if(res)
 printf("Matched!\n");
 else
 printf("Not Matched\n");
 return 0;
 }*/
