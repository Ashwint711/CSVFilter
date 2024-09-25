#include<stdio.h>
#include<string.h>
#include "headers.h"

int filter_rows(char **constraints, FILE *fp)
{
	int line_count = count_lines(fp);
	line_count -= 1;
	char line[1024];
	char result_line[1024];
	char ch;
	unsigned char str[50];

	char *key;
	char *value;

	//Output File
	FILE *output;
	output = fopen(constraints[0], "w");
	if(output == NULL)
	{
		printf("Filter-Rows : Failed to open output file!\n");
	}

	if(fgets(line, sizeof(line), fp) != NULL)
	{
		fprintf(output, "%s", line);
	}
	else
	{
		printf("Filter rows : Something went wrong!\n");
		return -1;
	}

	fseek(fp, 0, SEEK_SET);

	for(int i = 0; i < line_count; i++)
	{
		bool constraint_matched = true;
		int k = 0;
		if(constraints[k] != NULL)
		{
			strcpy(str, constraints[k]);
			trim_whitespace(str);


			//Use First row to calculate comma_count always
			fseek(fp, 0, SEEK_SET);
			int comma_count = 0;
			if(fgets(line, sizeof(line), fp) != NULL)
			{

				key = strtok(line, ",");
				while(key != NULL)
				{
					if(strcmp(trim_whitespace(key), str) == 0)
					{
						break;//Got commas till constraint.
					}

					comma_count += 1;
					trim_whitespace(key);

					key = strtok(NULL, ",");
				}
			}

			fseek(fp, 0, SEEK_SET);
			//Fetching new data row each time.
			for(int j = 0; j <= i; j++)
			{
				while((ch = fgetc(fp)) != '\n')
				{}
			}

			//Get value after comma_count commas
			if(fgets(line, sizeof(line), fp) != NULL)
			{
				strcpy(result_line, line);

				value = strtok(line, ",");
				while(value != NULL)
				{
					comma_count -= 1;
					if(comma_count < 0)
						break;

					value = strtok(NULL, ",");
				}


				trim_whitespace(value);
				//If value doesnt match than break iteratiion for current row and fetch next row.
				if(strcmp(value, trim_whitespace(constraints[k + 1])) != 0)
				{
					constraint_matched = false;
				}
			}
		}


		if(constraint_matched)
		{
			fprintf(output, "%s",result_line);
		}

	}

	fclose(output);
}
