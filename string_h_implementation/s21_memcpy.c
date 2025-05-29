#include "s21_string.h"

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  const unsigned char *ptr_src = src;
  unsigned char *ptr_dest = dest;
  while (n--) {
    *ptr_dest++ = *ptr_src++;
  }
  return ((void *)dest);
}