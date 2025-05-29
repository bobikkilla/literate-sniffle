#ifndef S21_GREP_H
#define S21_GREP_H

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct grep_options {
  int flag_i;           // register ignore
  int flag_v;           // invert grep (output NON pattern strings)
  int flag_c;           // outputs only count
  int flag_l;           // outputs only filenames
  int flag_n;           // output in format "(filename):(string num):(string)"
  int flag_h;           // output with no filename and string num, only string
  int flag_o;           // outputs only patterns ?)
  int flag_s;           // no errors about non exist or not opened files
  int count_filenames;  // no need to comment
  int count_patterns;   // no need to comment

} grep_options;

// parsing function
int parse_grep_arguments(int argc, char *argv[], char ***filenames_origin,
                         char **patterns_origin, grep_options **origin);

// support function for flags parsing
void flags_processing(grep_options *grep_flags, int i, char **argv, int argc,
                      int *status_to_return, int *pattern_status);

// support function for patterns parsing
void patterns_processing(int *pattern_status, int *count_patterns, char **argv,
                         char *patterns, int *status_to_return, int i);

int patterns_to_regex(regex_t *origin, char *pattern, int ignore_case_flag);

// transform all patterns into 1 string
char *concat_patterns(char **array, int count_patterns);

void output_processing(grep_options grep_flags, regex_t patterns,
                       char **filenames);
void another_output_func(grep_options grep_flags, regex_t patterns,
                         FILE *filestream, char *filenames);

void vflag_another_output_func(grep_options grep_flags, regex_t patterns,
                               FILE *filestream, char *filenames);

// just support function because >50 lines bruh
void real_trap_shit(char *nigger, grep_options grep_flags, regex_t patterns,
                    FILE *filestream, char *filename, regmatch_t match,
                    int *matched_count_string, int *count_string);

#endif
