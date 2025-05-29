#include "s21_string.h"

char *s21_strstr(const char *str1, const char *str2) {
  char *ptr = s21_NULL;
  char *tmp_str1 = (char *)str1;
  char *tmp_str2 = (char *)str2;
  if (*str2 == '\0') {
    ptr = (char *)str1;
  }
  while (*str1 != '\0' && !ptr) {
    tmp_str1 = (char *)str1;
    tmp_str2 = (char *)str2;
    while (*tmp_str1 == *tmp_str2) {
      tmp_str1++;
      tmp_str2++;
      if (*tmp_str2 == '\0') {
        ptr = (char *)str1;
      }
    }
    str1++;
  }
  return ptr;
}
