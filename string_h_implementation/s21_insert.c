#include "s21_string.h"

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  char *res = s21_NULL;
  int status = 0;
  if (src == s21_NULL || str == s21_NULL || s21_strlen(src) < start_index ||
      (s21_strlen(src) == 0 && start_index != 0))
    status = 1;

  if (!status) {
    s21_size_t src_len = s21_strlen(src);
    s21_size_t str_len = s21_strlen(str);
    s21_size_t new_len = src_len + str_len;

    res = calloc(new_len + 1, sizeof(char));

    if (res) {
      for (s21_size_t i = 0, j = 0, m = 0; i < new_len; i++) {
        if (i < start_index || !str[m])
          res[i] = src[j++];
        else
          res[i] = str[m++];
      }
      res[new_len] = '\0';
    }
  }
  return (void *)res;
}
