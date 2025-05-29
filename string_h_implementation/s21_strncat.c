#include "s21_string.h"

char *s21_strncat(char *dest, const char *src, size_t n) {
  char *ptr = dest;
  while (*ptr) ptr++;
  while (n-- && *src) *ptr++ = *src++;
  *ptr = '\0';
  return dest;
}
