#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "headers.h"


int main(int argc, void *args[])
{
	if(argc < 5)
	{
		perror("Invalid input!");
		return -1;
	}

	unsigned char **output_fields = get_output_fields(argc, args);

	//checking for "from" keyword
	unsigned char str[50];
	bool syntax = false;
	int file_index = -1;

	for(int i = 2; i < argc; i++)
	{
		strcpy(str, args[i]);
		trim_whitespace(str);

		if(!strcmp(str, "from") || !strcmp(str, "FROM") || !strcmp(str, "From"))
		{
			syntax = true;
			file_index = (i+1);
			break;
		}
		syntax = false;
	}

	if(!syntax)
	{
		perror("Wrong input syntax! refer [./select column_name1, column_name2 from file_name.csv where col1=<value> col2=<value>]\n");
		return -1;
	}


	//Accessing Input file
	unsigned char file_name[50];
	strcpy(file_name, args[file_index]);

	trim_whitespace(file_name);
	int len = strlen(file_name);

	if (len > 4 && strcmp(&file_name[len - 4], ".csv") != 0) {
		printf("Invalid file! ref: File should be of this <file_name.csv> format\n");
		return -1;
	}

	FILE *fp;
	fp = fopen(file_name, "r+");
	if(fp == NULL)
	{
		printf("File %s does not exist!\n",file_name);
		return -1;
	}

	//Checking output fields
	bool isValidOutputFields = validate_output_fields(output_fields, fp);

	if(!isValidOutputFields)
	{
		printf("Invalid Column name in select!\n");
		return -1;
	}

	//Checking where syntax
	strcpy(str, args[file_index+1]);
	trim_whitespace(str);

	if(strcmp(str, "where") != 0)
	{
		printf("syntax error : where caluse is missing\n");
		return -1;
	}


	//Getting Constraints
	unsigned char **constraints = get_constraints((file_index + 2), argc, args);

	//Checking constraint fields
	bool isValidConstraintFields = validate_constraint_fields(constraints, fp);

	if(!isValidConstraintFields)
	{
		printf("Invalid constraint name in select!\n");
		return -1;
	}


	//Passing Constraints to Filter-Rows
	char **constraint_value = (char**)malloc(sizeof(char) * 2);

	//Fetching index of last constraint-value pair
	int ret = 0, i = 0;
	while(constraints[i] != NULL)
	{
		i += 2;
	}
	i -= 2;

	while(i >=  0)
	{
		constraint_value[0] = constraints[i];
		constraint_value[1] = constraints[i+1];

		//Calling Filter-rows
		ret = filter_rows(constraint_value, fp);
		if(ret < 0)
		{
			printf("Something went wrong in filter-rows!\n");
			exit(0);
		}
		
		fclose(fp);
		fp = fopen(constraint_value[0], "r");
		if(fp == NULL)
		{
			printf("main : Failed to open %s file!\n", constraint_value[0]);
			return -1;
		}
		i -= 2;
	}


	//Printing Values from final Filtered file


	FILE *inp_file;
	inp_file = fopen(file_name, "r");
	if(inp_file == NULL)
	{
		printf("main : Failed to open file!\n");
		return -1;
	}

	fp = fopen(constraints[0], "r");
	if(fp == NULL)
	{
		printf("main : Failed to final Filtered file!\n");
		return -1;
	}

	int lines = count_lines(fp);
	lines -= 1;

	char line[1024];
	char entry[1024];

	int skip_offset = 0;
	char *word;
	char *subtoken;	

	bool flag = false;
	unsigned char colname[50];
	strcpy(colname, output_fields[0]);

	fgets(line, sizeof(line), fp);

	//Calculating skip offset
	fgets(line, sizeof(line), inp_file);
	fseek(inp_file, 0, SEEK_SET);


	char *ln = line;
	word = strtok_r(ln, ",", &ln);

	while(word != NULL)
	{
		trim_whitespace(word);

		subtoken = strtok(word, " ");
		while(subtoken != NULL)
		{
			if(strcmp(colname, subtoken) != 0)
			{
				skip_offset += 1;
			}
			else
			{
				flag = true;
				break;
			}


			subtoken = strtok(NULL, " ");
		}

		if(flag)
			break;

		word = strtok_r(NULL, ",", &ln);
	}



	for(int i = 0; i < lines; i++)
	{
		
		//Printing Actual Result
		fgets(entry, sizeof(entry), fp);
		
		int j = 0;
		flag = false;
		ln = entry;
		word = strtok_r(ln, ",", &ln);

		while(word != NULL)
		{
			trim_whitespace(word);

			subtoken = strtok(word, " ");
			while(subtoken != NULL)
			{
				if(j == skip_offset)
				{
					printf("%s\n",subtoken);
					flag = true;
				}
				j += 1;
				subtoken = strtok(NULL, " ");
			}

			if(flag)
				break;

			word = strtok_r(NULL, ",", &ln);
		}



	}








	/*//Using constraints to fetch data;
	  int k = 0;
	  int line_count = count_lines(fp);
	  line_count -= 1;
	  char line[1024];
	  char result_line[1024];
	  char ch;


	  char *key;
	  char *value;

	//Output File
	FILE *output;
	output = fopen("output-file", "w");
	if(output == NULL)
	{
	printf("Failed to create output file!");
	return -1;
	}
	//Done creating output file

	for(int i = 0; i < line_count; i++)
	{
	bool constraint_matched = true;
	k = 0;
	while(constraints[k] != NULL)
	{
	strcpy(str, constraints[k]);
	//printf("constraint : %s\n",str);
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
	//printf("comma count : %d\n",comma_count);
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


//Moving index to Next Constraint
k += 2;

trim_whitespace(value);
//If value doesnt match than break iteratiion for current row and fetch next row.
//printf("value : %s \n",value);
if(strcmp(value, trim_whitespace(constraints[k-1])) != 0)
{
	//	printf("value : %s \n constraints[k-1] : %s\n",trim_whitespace(value), constraints[k-1]);
	constraint_matched = false;
	break;
}
}
}

if(constraint_matched)
{
	//fwrite(line, sizeof(char), sizeof(line), output);
	printf("%s\n",result_line);
	//printf("constraint matched\n");
}else
{
	//	printf("Constraint didnt matched\n");
}
}
*/
fclose(fp);

return 0;
}
