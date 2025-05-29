#include "s21_string.h"

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  char *ptr = dest;
  while (n-- && *src) {
    *ptr++ = *src++;
  }
  *ptr = '\0';
  return dest;
}
