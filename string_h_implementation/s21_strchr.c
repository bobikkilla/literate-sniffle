#include "s21_string.h"

char *s21_strchr(const char *str, int c) {
  char *res = s21_NULL;
  bool status = true;
  while (*str && status) {
    if (*str == c) {
      res = (char *)str;
      status = false;
    }
    str++;
  }
  if (*str == c) {
    res = (char *)str;
  }
  return res;
}