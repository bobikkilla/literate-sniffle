#include "s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  void *res = s21_NULL;
  if (n != 0) {
    const unsigned char *ptr = str;
    while (n-- && res == s21_NULL) {
      if (*ptr++ == c) res = ((void *)--ptr);
    }
  }
  return res;
}