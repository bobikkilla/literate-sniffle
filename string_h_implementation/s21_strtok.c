#include "s21_string.h"

int is_delim(char a, const char *delims) {
  int status = 0;
  while (delims != s21_NULL && *delims && !status) {
    if (a == *delims) {
      status = 1;
    }
    delims++;
  }
  return status;
}

char *s21_strtok(char *str, const char *delim) {
  static char *str_stat;
  char *str_ptr = (char *)str;
  char *res = s21_NULL;
  int status = 0;
  if (str == s21_NULL) str_ptr = str_stat;

  while (str != s21_NULL && is_delim(*str, delim)) {
    str++;
    str_ptr++;
  }
  while (str == s21_NULL && (str_stat != s21_NULL) &&
         is_delim(*str_stat, delim)) {
    str_stat++;
    str_ptr++;
  }
  while (*str_ptr != '\0' && res == s21_NULL) {
    status = 0;
    if (is_delim(*str_ptr, delim)) {
      status = 1;
      *str_ptr = '\0';
    }
    str_ptr++;
    if (status == 1 || *str_ptr == '\0') {
      res = str ? str : str_stat;
      str_stat = str_ptr;
    }
  }
  return res;
}
