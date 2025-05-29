#include "s21_grep.h"

// first part of program is parcing
// second part we need to pass through regex all patterns
// third part we dealing with all flags and outputing

int main(int argc, char *argv[]) {
  grep_options *grep_flags = NULL;
  regex_t regex_compilied_patterns;
  char **filenames = NULL;
  char *total_pattern = NULL;  // for total big pattern
  // data initialization

  if (argc < 3) {
    printf("s21_grep: Not enough arguments\n");
    exit(0);
  }  // instant quit when no arguments

  int status_of_program =
      parse_grep_arguments(argc, argv, &filenames, &total_pattern, &grep_flags);
  if (status_of_program) {
    status_of_program *= patterns_to_regex(&regex_compilied_patterns,
                                           total_pattern, grep_flags->flag_i);
  }

  if (status_of_program) {
    if (grep_flags->count_filenames < 2) {
      grep_flags->flag_h = 1;
    }  // just logic of real grep(1 file = no filenames printed)
    output_processing(*grep_flags, regex_compilied_patterns, filenames);
    regfree(&regex_compilied_patterns);
  }  // if first&second part is ok, we enter this if construction

  if (total_pattern != NULL) free(total_pattern);
  if (filenames != NULL) free(filenames);
  if (grep_flags != NULL) free(grep_flags);
  // ending the program, freeing allocated memory
  return 0;
}

int parse_grep_arguments(int argc, char *argv[], char ***filenames_origin,
                         char **patterns_origin, grep_options **origin) {
  int status_to_return = 1;
  int pattern_status = 1;  // while pattern not found, it's 0
  grep_options *grep_flags = calloc(1, sizeof(grep_options));
  char **filenames = calloc(250, sizeof(char));
  char *patterns = calloc(1, sizeof(char));
  int count_patterns = 0, count_filenames = 0;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && argv[i][1] != '\0') {  // dealing with flags
      flags_processing(grep_flags, i, argv, argc, &status_to_return,
                       &pattern_status);
    } else {  // if dealing with non flags
      if (pattern_status) {
        if (count_patterns == 0) {
          patterns = realloc(patterns, 1 + sizeof(char) * strlen(argv[i]));
          patterns_processing(&pattern_status, &count_patterns, argv, patterns,
                              &status_to_return, i);
        } else {
          patterns = realloc(patterns, (strlen(patterns) + 2) +
                                           (sizeof(char) * strlen(argv[i])));
          patterns_processing(&pattern_status, &count_patterns, argv, patterns,
                              &status_to_return, i);
        }
      }  // dealing with patterns (need to add realloc)
      else {
        filenames = realloc(filenames, sizeof(char *) * (count_filenames + 1));
        if (filenames == NULL) {  // error allocating memory
          printf("s21_grep: error allocating memory(filenames)\n");
          status_to_return = 0;
        } else {
          filenames[count_filenames] = argv[i];
          count_filenames++;
        }
      }  // dealing with filenames (need to add realloc)
    }
    if (status_to_return == 0) {
      break;
    }  // if something gone wrong, we end cycle and quiting
  }
  grep_flags->count_filenames = count_filenames;
  grep_flags->count_patterns = count_patterns;
  *origin = grep_flags;
  *filenames_origin = filenames;
  *patterns_origin = patterns;
  return status_to_return;  // everything parced and data saved
}

void patterns_processing(int *pattern_status, int *count_patterns, char **argv,
                         char *patterns, int *status_to_return, int i) {
  if (*count_patterns == 0) {
    if (patterns == NULL) {
      printf("s21_grep: error allocating memory(patterns)\n");
      *status_to_return = 0;
    } else {
      strcat(patterns, argv[i]);
      (*count_patterns)++;
      *pattern_status = 0;
    }
  } else {
    if (patterns == NULL) {
      printf("s21_grep: error allocating memory(patterns)\n");
      *status_to_return = 0;
    } else {
      strcat(patterns, "|");
      strcat(patterns, argv[i]);
      (*count_patterns)++;
      *pattern_status = 0;
    }
  }
}

void flags_processing(grep_options *grep_flags, int i, char **argv, int argc,
                      int *status_to_return, int *pattern_status) {
  for (int j = 1; j < (int)strlen(argv[i]); j++) {
    switch (argv[i][j]) {
      case 'i':
        grep_flags->flag_i = 1;
        break;
      case 'v':
        grep_flags->flag_v = 1;
        break;
      case 'c':
        grep_flags->flag_c = 1;
        break;
      case 'l':
        grep_flags->flag_l = 1;
        break;
      case 'n':
        grep_flags->flag_n = 1;
        break;
      case 'h':
        grep_flags->flag_h = 1;
        break;
      case 's':
        grep_flags->flag_s = 1;
        break;
      case 'o':
        grep_flags->flag_o = 1;
        break;
      case 'e':
        if (i + 1 == argc) {
          printf("s21_grep: option requires an argument -- 'e'\n");
          *status_to_return = 0;
        } else {
          *pattern_status = 1;
        }
        break;
      default:
        printf("s21_grep: invalid option -- '%c'\n", argv[i][j]);
        *status_to_return = 0;
        break;
    }
  }
}

int patterns_to_regex(regex_t *origin, char *pattern, int ignore_case_flag) {
  int status = 1;
  if (ignore_case_flag) {
    if (regcomp(origin, pattern, REG_ICASE | REG_EXTENDED) != 0) {
      status = 0;
      printf("s21_grep: An error occured while compiling regex\n");
    }
  }  // if flag_i = 1
  else {
    if (regcomp(origin, pattern, REG_EXTENDED) != 0) {
      status = 0;
      printf("s21_grep: An error occured while compiling regex\n");
    }
  }  // if flag_i = 0
  return status;
}  // just compiling all patterns into regex

void output_processing(grep_options grep_flags, regex_t patterns,
                       char **filenames) {
  for (int i = 0; i < grep_flags.count_filenames; i++) {
    FILE *file = fopen(filenames[i], "r");
    if (file == NULL) {
      if (grep_flags.flag_s == 0) {
        printf("s21_grep: %s: No such file or directory\n", filenames[i]);
      }
    }  // no file error
    else {
      if (grep_flags.flag_v) {
        vflag_another_output_func(grep_flags, patterns, file, filenames[i]);
      } else {
        another_output_func(grep_flags, patterns, file, filenames[i]);
      }
      fclose(file);
    }
  }
}

void vflag_another_output_func(grep_options grep_flags, regex_t patterns,
                               FILE *filestream, char *filename) {
  regmatch_t match;
  char *nigger = calloc(2048, sizeof(char));
  int count_string = 1;
  int matched_count_string = 0;

  while (fgets(nigger, 2047, filestream) != NULL) {
    if (regexec(&patterns, nigger, 1, &match, 0) != 0) {
      if (grep_flags.flag_l) {
        printf("%s\n", filename);
        break;
      }  // if flag -l -> instant return
      if (grep_flags.flag_c) {
        matched_count_string++;
        continue;
      }  // if flag -c -> nothing to print, just continue
      if (!grep_flags.flag_o) {
        if (!grep_flags.flag_h) {
          printf("%s:", filename);
        }
        if (grep_flags.flag_n) {
          printf("%d:", count_string);
        }
        printf("%s", nigger);
        if (nigger[strlen(nigger) - 1] != '\n') {
          printf("\n");
        }
      }
    }
    count_string++;
  }
  if (grep_flags.flag_c && !grep_flags.flag_l) {
    if (!grep_flags.flag_h) {
      printf("%s:%d\n", filename, matched_count_string);
    } else {
      printf("%d\n", matched_count_string);
    }
  }

  free(nigger);  // hahaha
}

void another_output_func(grep_options grep_flags, regex_t patterns,
                         FILE *filestream, char *filename) {
  regmatch_t match;
  char *nigger = calloc(2048, sizeof(char));
  int count_string = 1;
  int matched_count_string = 0;
  real_trap_shit(nigger, grep_flags, patterns, filestream, filename, match,
                 &matched_count_string, &count_string);
  if (grep_flags.flag_c && (!grep_flags.flag_l)) {
    if (!grep_flags.flag_h) {
      printf("%s:%d\n", filename, matched_count_string);
    } else {
      printf("%d\n", matched_count_string);
    }
  }
  free(nigger);
}

void real_trap_shit(char *nigger, grep_options grep_flags, regex_t patterns,
                    FILE *filestream, char *filename, regmatch_t match,
                    int *matched_count_string, int *count_string) {
  while (fgets(nigger, 2047, filestream) != NULL) {
    if (!regexec(&patterns, nigger, 1, &match, 0)) {
      if (grep_flags.flag_l) {
        printf("%s\n", filename);
        break;
      }  // if flag -l -> instant return
      if (grep_flags.flag_c) {
        (*matched_count_string)++;
        continue;
      }  // if flag -c -> nothing to print, just continue
      if (grep_flags.flag_o) {
        if (!grep_flags.flag_h) {
          printf("%s:", filename);
        }
        if (grep_flags.flag_n) {
          printf("%d:", (*count_string));
        }
        for (int i = 0; i < (int)strlen(nigger); i++) {
          if (i >= match.rm_so && i < match.rm_eo) {
            printf("%c", nigger[i]);
          }
        }
        if (nigger[match.rm_eo - 1] != '\n') {
          printf("\n");
        }
      } else {
        if (!grep_flags.flag_h) {
          printf("%s:", filename);
        }
        if (grep_flags.flag_n) {
          printf("%d:", (*count_string));
        }
        printf("%s", nigger);
        if (nigger[strlen(nigger) - 1] != '\n') {
          printf("\n");
        }
      }
    }
    (*count_string)++;
  }
}