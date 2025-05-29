#include "s21_string.h"

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  int res = 0;
  while (n-- && res == 0) {
    if (*str1++ != *str2++) {
      res = *--str1 - *--str2;
    }
  }
  return res;
}
