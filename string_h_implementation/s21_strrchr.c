#include "s21_string.h"

char *s21_strrchr(const char *str, int c) {
  char *str1 = (char *)str;
  char *result = s21_NULL;
  if (str != s21_NULL) {
    for (int i = 0; i < (int)s21_strlen(str) + 1; i++, str1++) {
      if (*str1 == (char)c) result = str1;
    }
  }
  return result;
}