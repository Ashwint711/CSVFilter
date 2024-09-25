#include<stdbool.h>

#ifndef HEADER_H

#define HEADER_H

unsigned char** get_constraints(int start, int argc, void *args[]);

unsigned char** get_output_fields(int argc, void *args[]);

bool validate_output_fields(unsigned char** output_fields, FILE *fp);

bool validate_constraint_fields(unsigned char** output_fields, FILE *fp);

char *trim_whitespace(char *str);

unsigned long long int count_lines(FILE *fp);

int filter_rows(char **constraints, FILE *fp);

#endif
