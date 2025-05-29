
#include "s21_string.h"
typedef struct FLAGS {
  int flag_plus;
  int flag_minus;
  int flag_space;
  int flag_sharp;
  int flag_null;
  int start_count;
  int double_flag;
  int long_flag;
  int short_flag;
  int width;
  int accuracy;
  int accuracy_changed;
} FLAGS;
int parse_len(const char **format, FLAGS *flags);
int parse_floats(char **s, const char *format, FLAGS *flags, va_list *args,
                 int *count);
int parse_integers(char **s, const char *format, FLAGS *flags, va_list *args,
                   int *count);
int parse_strings(char **s, const char *format, FLAGS *flags, va_list *args,
                  int *count);
int print_float(char ***s, int **count, va_list **args, FLAGS **flags,
                int mode);
int print_str(char ***s, int **count, va_list **args, FLAGS *flags);
int print_char(char ***s, int **count, va_list **args, FLAGS *flags);
int print_wstring(char ***s, int **count, va_list **args, FLAGS *flags);
int print_wchar(char ***s, int **count, va_list **args, FLAGS *flags);
int parse_flags(const char **format, FLAGS *flags, va_list *args);
int flush_flags(FLAGS *flags);
int print_system(char **a, int base, int *i);
int sign_parsing(char **a, int *i, int sign, FLAGS **flags);
int print_pads(char ****s, FLAGS **flags, int ***count, int *len);
int int_to_str(long long mantissa, char ***a, int accuracy, int **i, int mode);
int power_parse(char ***a, int **i, int power);

#define BASE_TO(type, funcname)                                                \
  int base_to__##funcname(char **a, int base, type num, int *i, FLAGS **flags, \
                          int sign) {                                          \
    type n = num;                                                              \
    char *decimal_pool = "0123456789abcdef";                                   \
    char *high_decimal_pool = "0123456789ABCDEF";                              \
    char *current_pool = decimal_pool;                                         \
                                                                               \
    if (base == 17) base = base - 1;                                           \
    if (base == 18) {                                                          \
      base = base - 2;                                                         \
      current_pool = high_decimal_pool;                                        \
    }                                                                          \
    if (n / base != 0 || !((*flags)->accuracy_changed)) {                      \
      do {                                                                     \
        **a = current_pool[n % base];                                          \
        (*a)++;                                                                \
        (*i)++;                                                                \
      } while ((n /= base) > 0);                                               \
    }                                                                          \
    if ((*flags)->flag_null || ((*flags)->accuracy_changed == 1)) {            \
      int limit = (*flags)->accuracy;                                          \
      if (!(*flags)->accuracy_changed) limit = (*flags)->width;                \
      if ((!sign || (sign && (*flags)->flag_plus)) &&                          \
          (*flags)->accuracy_changed)                                          \
        limit++;                                                               \
      while (*i < limit) {                                                     \
        **a = '0';                                                             \
        (*a)++;                                                                \
        (*i)++;                                                                \
      }                                                                        \
    }                                                                          \
    return 0;                                                                  \
  }

BASE_TO(int, int)
BASE_TO(long int, lint)
BASE_TO(long long, llint)
BASE_TO(unsigned int, uint)
BASE_TO(unsigned long, luint)
BASE_TO(long long unsigned, lluint)

#define PRINT_INT(type, funcname, neg_check)                                   \
  int print__##funcname(char ***s, int **count, va_list **args, FLAGS **flags, \
                        int base) {                                            \
    int len = 0;                                                               \
    type num = va_arg(**args, type);                                           \
    if ((*flags)->short_flag == 1 && *#funcname == 'i') {                      \
      num = (short)num;                                                        \
    } else if ((*flags)->short_flag == 1) {                                    \
      num = (short unsigned)num;                                               \
    }                                                                          \
    int sign = 1, i = 0;                                                       \
    if ((*flags)->flag_plus && num > 0) i++;                                   \
    if ((*flags)->flag_plus && num == 0) i++;                                  \
    if ((long long)num <= 0 && neg_check) {                                    \
      if (num != 0) {                                                          \
        sign = 0;                                                              \
        num = num * -1;                                                        \
        i++;                                                                   \
      }                                                                        \
    }                                                                          \
    char *a = malloc(sizeof(char) * 256);                                      \
    base_to__##funcname(&a, base, num, &i, flags, sign);                       \
    if (((*flags)->flag_sharp && num != 0) || base == 17)                      \
      print_system(&a, base, &i);                                              \
    len = i;                                                                   \
    sign_parsing(&a, &i, sign, flags);                                         \
    if ((*flags)->flag_space && sign && !((*flags)->flag_plus) &&              \
        ((*flags)->width == len || (*flags)->width == 0)) {                    \
      ***s = ' ';                                                              \
      (**s)++;                                                                 \
      (**count)++;                                                             \
    }                                                                          \
    if ((*flags)->width != 0 && !((*flags)->flag_minus) &&                     \
        !((*flags)->flag_null))                                                \
      print_pads(&s, flags, &count, &len);                                     \
    while (i != -1) {                                                          \
      ***s = (*a);                                                             \
      a--;                                                                     \
      i--;                                                                     \
      (**count)++;                                                             \
      (**s)++;                                                                 \
    }                                                                          \
    if ((*flags)->width != 0 && ((*flags)->flag_minus)) {                      \
      print_pads(&s, flags, &count, &len);                                     \
    }                                                                          \
    a++;                                                                       \
    free(a);                                                                   \
    return 0;                                                                  \
  }

PRINT_INT(int, int, 1)
PRINT_INT(long int, lint, 1)
PRINT_INT(long long, llint, 1)
PRINT_INT(unsigned int, uint, 0)
PRINT_INT(unsigned long, luint, 0)
PRINT_INT(unsigned long long, lluint, 0)

#define SCIENTIFIC_NOTATION(type, funcname)                                \
  int scientific_notation##funcname(type num, FLAGS **flags, int accuracy, \
                                    char **a, int *i, int mode) {          \
    int power = 0, j = 2;                                                  \
    char exp_char = 'e';                                                   \
    if (mode == 3 || mode == 5) exp_char = 'E';                            \
    if (num != 0.0) {                                                      \
      while ((num) >= 10.0) {                                              \
        num /= 10.0;                                                       \
        power++;                                                           \
      }                                                                    \
      while ((num) < 1.0) {                                                \
        num *= 10.0;                                                       \
        power--;                                                           \
      }                                                                    \
    }                                                                      \
    double exponent = num - (double)(long long)num;                        \
    int tmp = power < 0 ? -power : power;                                  \
    while (j--) {                                                          \
      **a = tmp % 10 + '0';                                                \
      (*a)++;                                                              \
      (*i)++;                                                              \
      tmp /= 10;                                                           \
    }                                                                      \
    power_parse(&a, &i, power);                                            \
    **a = exp_char;                                                        \
    (*a)++;                                                                \
    (*i)++;                                                                \
    if ((*flags)->accuracy_changed) accuracy = (*flags)->accuracy;         \
    if ((mode == 4 || mode == 5) && !(*flags)->flag_sharp) {               \
      while ((long long)exponent % 10 == 0 && 0 < accuracy) {              \
        exponent /= 10;                                                    \
        accuracy--;                                                        \
      }                                                                    \
    } else if (mode == 4 || mode == 5)                                     \
      accuracy--;                                                          \
    for (int j = 0; j < accuracy; j++) exponent = exponent * 10;           \
    if (accuracy) {                                                        \
      int_to_str((long long)exponent, &a, accuracy, &i, 0);                \
      **a = '.';                                                           \
      (*a)++;                                                              \
      (*i)++;                                                              \
    }                                                                      \
    int_to_str((long long)num, &a, 0, &i, 1);                              \
    return 0;                                                              \
  }

SCIENTIFIC_NOTATION(double, float)
SCIENTIFIC_NOTATION(long double, lfloat)

#define FLOATING_POINT(type, funcname)                                \
  int floating_point##funcname(type num, FLAGS **flags, int accuracy, \
                               char **a, int *i, int mode) {          \
    long long mantissa = (long long)num;                              \
    type exponent = num - (type)mantissa;                             \
    int k = 0;                                                        \
    if ((*flags)->accuracy_changed == 1) {                            \
      accuracy = (*flags)->accuracy;                                  \
    }                                                                 \
    for (int j = 0; j < accuracy; j++) exponent = exponent * 10.0;    \
    if ((mode == 4 || mode == 5) && !(*flags)->flag_sharp) {          \
      while ((long long)exponent % 10 == 0 && k < accuracy) {         \
        exponent /= 10L;                                              \
        accuracy--;                                                   \
      }                                                               \
    }                                                                 \
    if (accuracy) {                                                   \
      int_to_str((long long)exponent, &a, accuracy, &i, 0);           \
      **a = '.';                                                      \
      (*a)++;                                                         \
      (*i)++;                                                         \
    }                                                                 \
    int_to_str(mantissa, &a, 0, &i, 1);                               \
    return 0;                                                         \
  }

FLOATING_POINT(double, float)
FLOATING_POINT(long double, lfloat)

#define PRINT_FLOAT(type, funcname, c)                                         \
  int print__##funcname(char ***s, int **count, va_list **args, FLAGS **flags, \
                        int mode) {                                            \
    char *a = malloc(512 * sizeof(char));                                      \
    type num = va_arg(**args, type);                                           \
    int accuracy = 6, i = 0, sign = 1;                                         \
    if (num < 0.0) {                                                           \
      sign = 0;                                                                \
      num = num * -1.0;                                                        \
      i++;                                                                     \
    } else if ((*flags)->flag_plus)                                            \
      i++;                                                                     \
    if (mode == 1)                                                             \
      floating_point##funcname(num, flags, accuracy, &a, &i, mode);            \
    if (mode == 2 || mode == 3)                                                \
      scientific_notation##funcname(num, flags, accuracy, &a, &i, mode);       \
    if (mode == 4 || mode == 5) {                                              \
      if (num >= 1000000.0 || num < 0.0001)                                    \
        scientific_notation##funcname(num, flags, accuracy, &a, &i, mode);     \
      else                                                                     \
        floating_point##funcname(num, flags, accuracy, &a, &i, mode);          \
    }                                                                          \
    sign_parsing(&a, &i, sign, flags);                                         \
    a++;                                                                       \
    i++;                                                                       \
    int len = i;                                                               \
    if ((*flags)->flag_space && sign && !((*flags)->flag_plus) &&              \
        ((*flags)->width == len || (*flags)->width == 0)) {                    \
      ***s = ' ';                                                              \
      (**s)++;                                                                 \
      (**count)++;                                                             \
    }                                                                          \
    if ((*flags)->width != 0 && !((*flags)->flag_minus) &&                     \
        !((*flags)->flag_null))                                                \
      print_pads(&s, flags, &count, &len);                                     \
    a--;                                                                       \
    while (i != 0) {                                                           \
      ***s = (*a);                                                             \
      a--;                                                                     \
      i--;                                                                     \
      (**s)++;                                                                 \
      (**count)++;                                                             \
    }                                                                          \
    if ((*flags)->width != 0 && ((*flags)->flag_minus)) {                      \
      print_pads(&s, flags, &count, &len);                                     \
    }                                                                          \
    a++;                                                                       \
    free(a);                                                                   \
    return 0;                                                                  \
  }

PRINT_FLOAT(double, float, 0)
PRINT_FLOAT(long double, lfloat, L)

/*int main() {
  char str1[1000];
  char str2[1000];

  char *format = "%f";
  double val = 15.35;

  int l = s21_sprintf(str1, format, val);
  int k = sprintf(str2, format, val);

  printf("%-s |count:%d \n", str1, l);
  printf("%-s |count:%d \n", str2, k);
  return 0;
}*/
int s21_sprintf(char *str, const char *format, ...) {
  char *s = str;
  FLAGS flags = {0};
  flags.accuracy = 1;
  int count = 0;
  va_list args;
  va_start(args, format);
  while (*format != '\0') {
    if (*format == '%') {
      format++;
      flags.start_count = count;
      parse_flags(&format, &flags, &args);
      parse_len(&format, &flags);
      parse_integers(&s, format, &flags, &args, &count);
      parse_floats(&s, format, &flags, &args, &count);
      parse_strings(&s, format, &flags, &args, &count);
      if (*format == '%') {
        *s = '%';
        s++;
        count++;
      }
      flush_flags(&flags);
      format++;
    } else {
      *s = *format;
      count++;
      s++;
      format++;
    }
  }
  *s = '\0';
  va_end(args);
  return count;
}

int sign_parsing(char **a, int *i, int sign, FLAGS **flags) {
  if (!sign) {
    **a = '-';
    (*a)++;
  } else if ((*flags)->flag_plus) {
    **a = '+';
    (*a)++;
  }
  (*i)--;
  (*a)--;
  return 0;
}
int print_str(char ***s, int **count, va_list **args, FLAGS *flags) {
  char *str = va_arg(**args, char *);
  int len = 0;
  char *tmp_str = malloc(sizeof(char) * 1024);
  char *ptr = tmp_str;
  char *src_ptr = tmp_str;
  while (*str != '\0') {
    *ptr = (*str);
    ptr++;
    str++;
    len++;
  }
  int len_s = len;
  if (flags->width != 0 && !(flags->flag_minus))
    print_pads(&s, &flags, &count, &len);
  if (len_s > flags->accuracy && flags->accuracy_changed)
    len_s = flags->accuracy;

  for (int i = 0; i < len_s; i++) {
    ***s = *src_ptr;
    src_ptr++;
    (**s)++;
    (**count)++;
  }
  if (flags->width != 0 && (flags->flag_minus))
    print_pads(&s, &flags, &count, &len_s);

  free(tmp_str);
  return 0;
}
int print_char(char ***s, int **count, va_list **args, FLAGS *flags) {
  char c = va_arg(**args, int);
  int len = 1;
  if (flags->width != 0 && !(flags->flag_minus))
    print_pads(&s, &flags, &count, &len);
  ***s = c;
  (**s)++;
  (**count)++;
  if (flags->width != 0 && (flags->flag_minus))
    print_pads(&s, &flags, &count, &len);
  return 0;
}
int print_wstring(char ***s, int **count, va_list **args, FLAGS *flags) {
  setlocale(LC_ALL, "");
  wchar_t *ws = va_arg(**args, wchar_t *);
  int len = 0, len_s = 0;
  char *tmp_str = malloc(sizeof(char) * 1024);
  char *ptr = tmp_str;
  char *src_ptr = tmp_str;
  while (*ws != L'\0') {
    char mbstr[MB_CUR_MAX];
    int len_c = wctomb(mbstr, *ws);
    for (int i = 0; i < len_c; i++) {
      *ptr = mbstr[i];
      ptr++;
    }
    len += len_c;
    ws++;
  }
  len_s = len;
  if (flags->width != 0 && !(flags->flag_minus))
    print_pads(&s, &flags, &count, &len);
  if (len_s > flags->accuracy && flags->accuracy_changed)
    len_s = flags->accuracy;
  for (int i = 0; i < len_s; i++) {
    ***s = *src_ptr;
    (**count)++;
    (**s)++;
    src_ptr++;
  }
  if (flags->width != 0 && (flags->flag_minus))
    print_pads(&s, &flags, &count, &len);
  free(tmp_str);
  return 0;
}
int print_wchar(char ***s, int **count, va_list **args, FLAGS *flags) {
  setlocale(LC_ALL, "");
  wchar_t wc = va_arg(**args, wchar_t);
  char mbstr[MB_CUR_MAX];
  int len = wctomb(mbstr, wc);
  int len_c = len;
  if (flags->width != 0 && !(flags->flag_minus))
    print_pads(&s, &flags, &count, &len_c);
  for (int i = 0; i < len; i++) {
    ***s = mbstr[i];
    (**s)++;
    (**count)++;
  }
  if (flags->width != 0 && (flags->flag_minus))
    print_pads(&s, &flags, &count, &len_c);
  return 0;
}
int parse_flags(const char **format, FLAGS *flags, va_list *args) {
  while (1) {
    if (**format == '-')
      flags->flag_minus = 1;
    else if (**format == '+')
      flags->flag_plus = 1;
    else if (**format == ' ') {
      if (!(flags->flag_plus)) flags->flag_space = 1;
    } else if (**format == '#')
      flags->flag_sharp = 1;
    else if (**format == '0')
      flags->flag_null = 1;
    else if (**format == '*')
      flags->width = va_arg(*args, int);
    else if (**format > 47 && **format < 58) {
      while (**format > 47 && **format < 58) {
        flags->width = flags->width * 10 + (**format - '0');
        (*format)++;
      }
      (*format)--;
    } else if (**format == '.') {
      (*format)++;
      flags->accuracy = 0;
      flags->accuracy_changed = 1;
      int stat = 1;
      while ((stat != 2) && ((**format > 47 && **format < 58) ||
                             (**format == '*' && flags->accuracy == 0))) {
        stat = 0;
        if (**format == '*') {
          flags->accuracy = va_arg(*args, int);
          stat = 2;
        } else
          flags->accuracy = flags->accuracy * 10 + (**format - '0');
        (*format)++;
      }
      if (stat == 1) flags->accuracy = 0;
      (*format)--;
    } else
      break;
    (*format)++;
  }
  return 0;
}
int parse_len(const char **format, FLAGS *flags) {
  if (**format == 'l') {
    flags->long_flag = 1;
    (*format)++;
    if (**format == 'l') {
      flags->long_flag = 2;
      (*format)++;
    }
  } else if (**format == 'h') {
    flags->short_flag = 1;
    (*format)++;
  } else if (**format == 'L') {
    flags->double_flag = 1;
    (*format)++;
  }
  return 0;
}
int parse_integers(char **s, const char *format, FLAGS *flags, va_list *args,
                   int *count) {
  if (flags->long_flag == 1) {
    if (*format == 'd' || *format == 'i')
      print__lint(&s, &count, &args, &flags, 10);
    if (*format == 'o') print__luint(&s, &count, &args, &flags, 8);
    if (*format == 'x') print__luint(&s, &count, &args, &flags, 16);
    if (*format == 'X') print__luint(&s, &count, &args, &flags, 18);
    if (*format == 'u') print__luint(&s, &count, &args, &flags, 10);
  } else if (flags->long_flag == 2) {
    if (*format == 'd' || *format == 'i')
      print__llint(&s, &count, &args, &flags, 10);
    if (*format == 'o') print__lluint(&s, &count, &args, &flags, 8);
    if (*format == 'x') print__lluint(&s, &count, &args, &flags, 16);
    if (*format == 'X') print__lluint(&s, &count, &args, &flags, 18);
    if (*format == 'u') print__lluint(&s, &count, &args, &flags, 10);
  } else {
    if (*format == 'd' || *format == 'i')
      print__int(&s, &count, &args, &flags, 10);
    if (*format == 'o') print__uint(&s, &count, &args, &flags, 8);
    if (*format == 'x') print__uint(&s, &count, &args, &flags, 16);
    if (*format == 'X') print__uint(&s, &count, &args, &flags, 18);
    if (*format == 'u') print__uint(&s, &count, &args, &flags, 10);
  }
  if (*format == 'p') print__lluint(&s, &count, &args, &flags, 17);

  return 0;
}
int parse_floats(char **s, const char *format, FLAGS *flags, va_list *args,
                 int *count) {
  if (flags->double_flag == 1) {
    if (*format == 'f') print__lfloat(&s, &count, &args, &flags, 1);
    if (*format == 'e') print__lfloat(&s, &count, &args, &flags, 2);
    if (*format == 'E') print__lfloat(&s, &count, &args, &flags, 3);
    if (*format == 'g') print__lfloat(&s, &count, &args, &flags, 4);
    if (*format == 'G') print__lfloat(&s, &count, &args, &flags, 5);
  } else {
    if (*format == 'f') print__float(&s, &count, &args, &flags, 1);
    if (*format == 'e') print__float(&s, &count, &args, &flags, 2);
    if (*format == 'E') print__float(&s, &count, &args, &flags, 3);
    if (*format == 'g') print__float(&s, &count, &args, &flags, 4);
    if (*format == 'G') print__float(&s, &count, &args, &flags, 5);
  }
  return 0;
}
int parse_strings(char **s, const char *format, FLAGS *flags, va_list *args,
                  int *count) {
  if (flags->long_flag == 1) {
    if (*format == 's') print_wstring(&s, &count, &args, flags);
    if (*format == 'c') print_wchar(&s, &count, &args, flags);
  } else {
    if (*format == 's') print_str(&s, &count, &args, flags);
    if (*format == 'c') print_char(&s, &count, &args, flags);
  }
  return 0;
}
int flush_flags(FLAGS *flags) {
  flags->accuracy = 1;
  flags->flag_minus = 0;
  flags->flag_null = 0;
  flags->flag_plus = 0;
  flags->flag_sharp = 0;
  flags->flag_space = 0;
  flags->long_flag = 0;
  flags->short_flag = 0;
  flags->width = 0;
  flags->accuracy_changed = 0;
  return 0;
}
int print_pads(char ****s, FLAGS **flags, int ***count, int *len) {
  while (*len < (*flags)->width) {
    ****s = ' ';
    ***count += 1;
    (*len)++;
    (***s)++;
  }
  return 0;
}

int print_system(char **a, int base, int *i) {
  if (base == 16 || base == 17) {
    **a = 'x';
    (*a)++;
    **a = '0';
    (*a)++;
    *i += 2;
  } else if (base == 8) {
    **a = '0';
    (*a)++;
    *i += 1;
  } else if (base == 18) {
    **a = 'X';
    (*a)++;
    **a = '0';
    (*a)++;
    *i += 2;
  }

  return 0;
}
int int_to_str(long long mantissa, char ***a, int accuracy, int **i, int mode) {
  do {
    ***a = mantissa % 10 + '0';
    (**a)++;
    (**i)++;
    mantissa /= 10;
  } while ((--accuracy > 0 && mode == 0) ||
           ((mantissa % 10) != 0 && mode == 1));
  return 0;
}
int power_parse(char ***a, int **i, int power) {
  if (power < 0) {
    ***a = '-';
    (**a)++;
    (**i)++;
  } else {
    ***a = '+';
    (**a)++;
    (**i)++;
  }
  return 0;
}