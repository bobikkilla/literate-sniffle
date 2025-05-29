#include "s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  int res = 0;
  if (n != 0) {
    const unsigned char *ptr1 = str1, *ptr2 = str2;
    while (n-- && res == 0) {
      if (*ptr1++ != *ptr2++) {
        res = *--ptr1 - *--ptr2;
      }
    }
  }
  return res;
}