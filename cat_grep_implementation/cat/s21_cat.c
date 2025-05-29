#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cat_flag_struct.h"

#define N 250

void printfile(char **filenames, cat_flags *flags, int count_filenames);

int arg_detect(int argc, char **argv, cat_flags **flags_origin,
               char ***filenames_origin, int *filenames_count);

void short_arg_parcing(char **argv, int *status, int i, cat_flags *flags);
void long_arg_parcing(char **argv, int *status, int i, cat_flags *flags);

void v_print(unsigned char c);

int main(int argc, char *argv[]) {
  cat_flags *flags = NULL;
  char **filenames = NULL;
  int filenames_count = 0;

  if (arg_detect(argc, argv, &flags, &filenames, &filenames_count)) {
    printfile(filenames, flags, filenames_count);
  }

  free(flags);
  free(filenames);
  return 0;
}

int arg_detect(int argc, char **argv, cat_flags **flags_origin,
               char ***filenames_origin, int *filenames_count) {
  char **filenames = calloc(250, sizeof(char *));
  cat_flags *flags = calloc(1, sizeof(struct cat_flags));
  int count_filenames = 0;
  int status = 1;  // status 1 = good, 0 = bad
  // creating temporary variables to throw values in original
  if (flags == NULL) {  // errors processing
    printf("s21_cat: memory allocation error(flags)");
    status = 0;
  }
  if (argc == 1) {
    printf("s21_cat: no arguments passed");
    status = 0;
  }

  int i = 1;
  while (i < argc && status == 1) {
    if (argv[i][0] == '-' && argv[i][1] == '-') {
      long_arg_parcing(argv, &status, i, flags);
    } else if (argv[i][0] == '-' && argv[i][1] != '-') {
      short_arg_parcing(argv, &status, i, flags);
    } else {
      filenames = realloc(filenames, sizeof(char *) * (count_filenames + 1));
      if (filenames == NULL) {
        printf("s21_cat: memory allocation error(filenames)");
        status = 0;
      } else {
        filenames[count_filenames] = argv[i];
        if (filenames[count_filenames] == NULL) {
          printf("s21_cat: memory allocation error(filenames)");
          status = 0;
        } else {
          count_filenames++;
        }
      }
    }
    i++;
  }
  if (flags->flag_b) flags->flag_n = 0;
  *flags_origin = flags;
  *filenames_origin = filenames;
  *filenames_count = count_filenames;
  return status;
}  // everything is good

void printfile(char **filenames, cat_flags *flags, int count_filenames) {
  int count = 1;
  int previousNumber = 10, freeStr = 0;
  for (int i = 0; i < count_filenames; i++) {
    FILE *file = fopen(filenames[i], "r");
    if (!file) {
      printf("\ns21_cat: %s: No such file or directory\n", filenames[i]);
      continue;
    }  // file is not opened

    int number;
    while ((number = fgetc(file)) != EOF) {
      int blankSrting = 0;
      if (previousNumber == 10 && number == 10) {
        freeStr++;
        blankSrting = 1;
      } else {
        blankSrting = 0;
        freeStr = 0;
      }
      if (flags->flag_s && freeStr > 1) {
        continue;
      }
      if ((flags->flag_n || (flags->flag_b && blankSrting == 0)) &&
          previousNumber == 10) {
        printf("%6d\t", count);
        count++;
      }
      if (flags->flag_T && number == 9) {
        printf("^I");
        previousNumber = 9;
        continue;
      }
      if (flags->flag_E && number == 10) printf("$");
      if (flags->flag_v)
        v_print(number);
      else
        fputc(number, stdout);

      previousNumber = number;
    }
    fclose(file);
  }
  return;
}

void v_print(unsigned char c) {
  if (c == 9 || c == 10) {
    printf("%c", c);
  } else if (c >= 32 && c < 127) {
    printf("%c", c);
  } else if (c == 127) {
    printf("^?");
  } else if (c >= 160) {
    printf("M-");
    (c < 255) ? printf("%c", c - 128) : printf("^?");
  } else {
    (c > 32) ? printf("M-^%c", c - 64) : printf("^%c", c + 64);
  }
}

void short_arg_parcing(char **argv, int *status, int i, cat_flags *flags) {
  switch (argv[i][1]) {
    case 'n':
      flags->flag_n = 1;
      break;
    case 'b':
      flags->flag_b = 1;
      break;
    case 's':
      flags->flag_s = 1;
      break;
    case 'e':
      flags->flag_E = 1;
      flags->flag_v = 1;
      break;
    case 't':
      flags->flag_T = 1;
      flags->flag_v = 1;
      break;
    case 'E':
      flags->flag_E = 1;
      break;
    case 'T':
      flags->flag_T = 1;
      break;
    case 'v':
      flags->flag_v = 1;
      break;
    default:
      printf("s21_cat: wrong argument\n");  // досрочно завершить
      *status = 0;
      break;
  }
}

void long_arg_parcing(char **argv, int *status, int i, cat_flags *flags) {
  if (!(strcmp(argv[i], "--number-nonblank"))) {
    flags->flag_b = 1;
  } else if (!(strcmp(argv[i], "--number"))) {
    flags->flag_n = 1;
  } else if (!(strcmp(argv[i], "--squeeze-blank"))) {
    flags->flag_s = 1;
  } else {
    printf("s21_cat: wrong argument\n");  // завершаем код с ошибкой
    *status = 0;
  }
}
