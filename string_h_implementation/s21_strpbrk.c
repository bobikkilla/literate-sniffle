#include "s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  char *res = s21_NULL;
  char *ptr = (char *)str2;
  bool status = true;
  while (*str1 && status) {
    if (*ptr) {
      if (*str1 == *ptr++) {
        res = (char *)str1;
        status = false;
      }
    } else {
      str1++;
      ptr = (char *)str2;
    }
  }
  return res;
}
