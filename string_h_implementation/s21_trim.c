#include "s21_string.h"

void *s21_trim(const char *src, const char *trim_chars) {
  char *res = s21_NULL;

  if (trim_chars == s21_NULL) {
    trim_chars = " ";
  }
  if (src) {
    int begin = 0;
    int end = s21_strlen(src);
    while (begin < end && s21_strchr(trim_chars, src[begin])) begin++;
    while (end > begin && s21_strchr(trim_chars, src[end - 1])) end--;
    res = (char *)calloc(end - begin + 1, sizeof(char));

    if (begin != 0)
      res[end - begin] = '\0';
    else
      res[end] = '\0';

    for (int i = 0; begin < end; i++, begin++) {
      res[i] = src[begin];
    }
  }
  return (void *)res;
}