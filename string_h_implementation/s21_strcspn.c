#include "s21_string.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t count = 0;
  char *ptr = (char *)str2;
  while (*str1) {
    if (*ptr) {
      if (*str1 == *ptr++) {
        break;
      }
    } else {
      str1++;
      count++;
      ptr = (char *)str2;
    }
  }
  return count;
}
