#include "s21_string.h"

// бля ну тут пилить нужно работу с строками, типа если дана строка больше чем
// нужно записать, ласт символы изначальной строки не переписываются
// и дальше чекать кто из спецификаторов хуёво работает

typedef struct modifiers_struct {
  bool ll;
  bool l;
  bool h;
  bool hh;
  bool suppress;
  bool space_for_chars;
} modifiers_struct;

enum data_specifiers { c, d, i, e, E, f, g, G, o, s, u, x, X, p, n };

/*sscanf functions*/

void format_parsing(const char **format, modifiers_struct *modifiers,
                    enum data_specifiers *specs, bool *program_status);
void nulling_format_data(enum data_specifiers *specs,
                         modifiers_struct *modifiers);
void char_specifier_processing(const char **str,
                               unsigned *count_of_success_symbols,
                               va_list *arguments,
                               enum data_specifiers specifiers,
                               modifiers_struct modifiers);
bool p_specifier_processing(const char **str, unsigned *success_scanned_symbols,
                            va_list *arguments, bool supress_flag);
void n_specifier_processing(va_list *arguments,
                            unsigned *success_scanned_symbols);
bool int_specifier_processing(const char **str,
                              unsigned *count_of_success_symbols,
                              va_list *arguments,
                              enum data_specifiers specifiers,
                              modifiers_struct modifiers);
bool str_to_num(const char **str, unsigned long long *num,
                unsigned *count_of_success_symbols, int base);
bool float_specifiers_processing(const char **str,
                                 unsigned int *count_of_success_symbols,
                                 va_list *arguments,
                                 modifiers_struct modifiers);
bool int_processing_final_part(long long num, int sign,
                               enum data_specifiers specifiers,
                               va_list *arguments, modifiers_struct modifiers);

void floatnum_to_argument(long double result, int sign, va_list *arguments,
                          modifiers_struct modifiers);
long double post_floatingpoint_processing(const char **str,
                                          unsigned *count_of_success_symbols);
bool scientific_notation_processing(const char **str,
                                    unsigned *count_of_success_symbols,
                                    long double *num);
bool is_hex(char a);
int hex_to_int(char a);
bool is_octal(char a);
int oct_to_int(char a);
bool is_num(char a);
void what_are_we_dealing_with_for_int(const char **str,
                                      enum data_specifiers specifiers,
                                      int *base);
bool main_part_processing(va_list *args, unsigned *success_scanned_count,
                          modifiers_struct modifiers, enum data_specifiers type,
                          const char **str);

int s21_sscanf(const char *str, const char *format, ...) {
  va_list arguments;
  va_start(arguments, format);
  unsigned count_of_success_symbols = 0;
  int count_of_success_variables = 0;
  bool program_status = 1;
  modifiers_struct modifiers;
  enum data_specifiers specs;
  nulling_format_data(&specs, &modifiers);
  // if format || str is empty
  if (str == s21_NULL || format == s21_NULL) {
    program_status = 0;
    count_of_success_variables = EOF;
  }
  while (program_status && *str && *format) {
    if (*format == '%') {
      format++;
      // parcing the format string token
      format_parsing(&format, &modifiers, &specs, &program_status);
      // processing if program status is true
      if (program_status) {
        program_status = main_part_processing(
            &arguments, &count_of_success_symbols, modifiers, specs, &str);
        (modifiers.suppress == 0) ? count_of_success_variables += program_status
                                  : count_of_success_variables;
        (specs == n) ? count_of_success_variables -= 1
                     : count_of_success_variables;
        nulling_format_data(&specs, &modifiers);
      }
    } else if (*format == ' ') {
      format++;
    } else {
      program_status = 0;
    }
  }
  va_end(arguments);
  return count_of_success_variables;
}

// parsing first argument from format string(which variable we will process)
// this function is big only because of switch case construction
// returns nothing, but takes in program status
void format_parsing(const char **format, modifiers_struct *modifiers,
                    enum data_specifiers *specs, bool *program_status) {
  // modifiers cycle
  while (**format == '*' || **format == 'h' || **format == 'l' ||
         **format == 'L') {
    if (**format == '*') {
      modifiers->suppress = 1;
    }  // if suppress symb found
    else if (**format == 'h') {
      if (**(format + 1) == 'h') {
        modifiers->hh = 1;
        (*format)++;
      } else {
        modifiers->h = 1;
      }
    } else if (**format == 'l') {
      if (**(format + 1) == 'l') {
        modifiers->ll = 1;
        (*format)++;
      } else {
        modifiers->l = 1;
      }
    } else if (**format == 'L') {
      modifiers->ll = 1;
    }
    (*format)++;
  }
  // switch case for specifier
  switch (**format) {
    case 'c':
      *specs = c;
      if (*(*format + 1) == ' ') {
        modifiers->space_for_chars = 1;
      }
      break;
    case 'd':
      *specs = d;
      break;
    case 'i':
      *specs = i;
      break;
    case 'e':
      *specs = e;
      break;
    case 'E':
      *specs = E;
      break;
    case 'f':
      *specs = f;
      break;
    case 'g':
      *specs = g;
      break;
    case 'G':
      *specs = G;
      break;
    case 'o':
      *specs = o;
      break;
    case 's':
      *specs = s;
      break;
    case 'u':
      *specs = u;
      break;
    case 'x':
      *specs = x;
      break;
    case 'X':
      *specs = X;
      break;
    case 'p':
      *specs = p;
      break;
    case 'n':
      *specs = n;
      break;
    default:
      *program_status = 0;
      break;
  }
  (*format)++;
}

// this function is entry point to whole proccess segment
// returns true or false, based on success or fail while processing
bool main_part_processing(va_list *arguments, unsigned *success_scanned_count,
                          modifiers_struct modifiers, enum data_specifiers type,
                          const char **str) {
  bool status_of_scanning = 1;
  // characters processing
  if (type == c || type == s) {
    if (modifiers.h || modifiers.ll || modifiers.l ||
        (modifiers.hh && type == s)) {
      status_of_scanning = 0;
    } else {
      char_specifier_processing(str, success_scanned_count, arguments, type,
                                modifiers);
    }

  } else if (type == d || type == i || type == o || type == u || type == x ||
             type == X) {
    // integer processing
    // here we can have everything except short short
    if (modifiers.hh) {
      status_of_scanning = 0;
    } else {
      status_of_scanning = int_specifier_processing(str, success_scanned_count,
                                                    arguments, type, modifiers);
    }
  } else if (type == f || type == e || type == E || type == g || type == G) {
    if (modifiers.h || modifiers.hh) {
      status_of_scanning = 0;
    } else {
      status_of_scanning = float_specifiers_processing(
          str, success_scanned_count, arguments, modifiers);
    }
  } else if (type == n) {
    n_specifier_processing(arguments, success_scanned_count);
  } else if (type == p) {
    if (modifiers.h || modifiers.hh || modifiers.l || modifiers.ll) {
      // if we get any modifier for p, status becomes failed
      status_of_scanning = 0;
    } else {
      status_of_scanning = p_specifier_processing(
          str, success_scanned_count, arguments, modifiers.suppress);
    }
  }
  return status_of_scanning;
}

// n specifier is used for giving the number of successfully scanned symbols
void n_specifier_processing(va_list *arguments,
                            unsigned *success_scanned_symbols) {
  int *num = va_arg(*arguments, int *);
  *num = *success_scanned_symbols;
}

// idk is this working or not :)
// pointer specifier is used for scanning address from string
bool p_specifier_processing(const char **str, unsigned *success_scanned_symbols,
                            va_list *arguments, bool supress_flag) {
  bool status_of_scanning = 1;
  while (**str <= 32) {
    (*str)++;
  }
  if (**str == '0' && *((*str) + 1) == 'x') {
    (*str) += 2;
    (*success_scanned_symbols) += 2;
    long unsigned int address = 0;
    while (is_hex(**str)) {
      address = address * 16 + hex_to_int(**str);
      (*str)++;
      (*success_scanned_symbols)++;
    }
    if (!supress_flag) {
      void **pointer = va_arg(*arguments, void **);
      *pointer = (void *)(address);
    }
  } else {
    status_of_scanning = 0;
  }
  return status_of_scanning;
}

// function for cleaning data for next variable processing
void nulling_format_data(enum data_specifiers *specs,
                         modifiers_struct *modifiers) {
  modifiers->h = 0;
  modifiers->hh = 0;
  modifiers->l = 0;
  modifiers->ll = 0;
  modifiers->suppress = 0;
  modifiers->space_for_chars = 0;
  *specs = 0;
}

// function for char&string processing
void char_specifier_processing(const char **str,
                               unsigned *count_of_success_symbols,
                               va_list *arguments,
                               enum data_specifiers specifiers,
                               modifiers_struct modifiers) {
  if (modifiers.suppress && specifiers == c) {
    (*str)++;
  } else if (modifiers.suppress && specifiers == s) {
    while (**str != ' ') {
      (*str)++;
    }
  } else if (specifiers == c && modifiers.hh) {
    char *a = va_arg(*arguments, char *);
    *a = **str;
    (*str)++;
    (*count_of_success_symbols)++;
  } else {
    if (specifiers == s || modifiers.space_for_chars) {
      while (**str == ' ' || **str == '\t') {
        (*str)++;
        (*count_of_success_symbols)++;
      }
    }
    // string or char
    char *ch = va_arg(*arguments, char *);
    do {
      *ch++ = **str;
      (*str)++;
      (*count_of_success_symbols)++;
    } while (specifiers && **str != ' ' && **str != '\t' && **str != '\0');
    if (specifiers == s) {
      while (ch != s21_NULL && *ch != '\0') {
        *ch = '\0';
      }
    }
  }
}

// hellish
// int specifier is for integer numbers
bool int_specifier_processing(const char **str,
                              unsigned *count_of_success_symbols,
                              va_list *arguments,
                              enum data_specifiers specifiers,
                              modifiers_struct modifiers) {
  while (**str <= 32) {
    (*str)++;
    (*count_of_success_symbols)++;
  }
  int base = 10;
  bool status = true;
  if (**str == '-' && (specifiers == u || specifiers == x || specifiers == X ||
                       specifiers == o)) {
    status = false;
  } else {
    int sign = 1;
    if (**str == '-' || **str == '+') {
      sign = (**str == '-') ? -1 : 1;
      (*str)++;
      (*count_of_success_symbols)++;
    }
    what_are_we_dealing_with_for_int(str, specifiers, &base);
    unsigned long long num = 0;
    status = str_to_num(str, &num, count_of_success_symbols, base);
    if (status && !modifiers.suppress) {
      status = int_processing_final_part(num, sign, specifiers, arguments,
                                         modifiers);
    }
  }
  return status;
}

bool str_to_num(const char **str, unsigned long long *num,
                unsigned *count_of_success_symbols, int base) {
  bool status = true;
  if (base == 16) {
    *count_of_success_symbols += 2;
    status = false;
    while (is_hex(**str)) {
      if (status == false && **str == '0') {
        num = 0;
        (*count_of_success_symbols)++;
        (*str)++;
        status = true;
        break;
      }
      *num = *num * base + hex_to_int(**str);
      (*count_of_success_symbols)++;
      (*str)++;
      status = true;
    }
  } else if (base == 8) {
    status = false;
    (*count_of_success_symbols)++;
    while (is_octal(**str)) {
      if (status == false && **str == '0') {
        num = 0;
        (*count_of_success_symbols)++;
        (*str)++;
        status = true;
        break;
      }
      *num = *num * base + oct_to_int(**str);
      (*count_of_success_symbols)++;
      (*str)++;
      status = true;
    }
  } else if (base == 10) {
    status = false;
    while (is_num(**str)) {
      if (status == false && **str == '0') {
        num = 0;
        (*count_of_success_symbols)++;
        (*str)++;
        status = true;
        break;
      }
      *num = *num * base + hex_to_int(**str);
      (*count_of_success_symbols)++;
      (*str)++;
      status = true;
    }
  }
  return status;
}
// second part for STRUCTURE PROGRAMMING
bool int_processing_final_part(long long num, int sign,
                               enum data_specifiers specifiers,
                               va_list *arguments, modifiers_struct modifiers) {
  bool status = false;
  if (specifiers == o || specifiers == x || specifiers == X ||
      specifiers == u) {
    if (modifiers.ll) {
      long long unsigned *number = va_arg(*arguments, long long unsigned *);
      *number = num;
    } else if (modifiers.l) {
      if ((long unsigned)num <= ULONG_MAX) {
        long unsigned *number = va_arg(*arguments, long unsigned *);
        *number = num;
        status = true;
      }
    } else if (modifiers.h) {
      if (num <= USHRT_MAX) {
        short unsigned *number = va_arg(*arguments, short unsigned *);
        *number = num;
        status = true;
      }
    } else {
      if (num <= UINT_MAX) {
        unsigned *number = va_arg(*arguments, unsigned *);
        *number = num;
        status = true;
      }
    }
  } else if (specifiers == d || specifiers == i) {
    if (modifiers.h) {
      if (num <= SHRT_MAX) {
        short *number = va_arg(*arguments, short *);
        *number = num * sign;
        status = true;
      }
    } else if (modifiers.l) {
      if (num <= LONG_MAX) {
        long *number = va_arg(*arguments, long *);
        *number = num * sign;
        status = true;
      }
    } else if (modifiers.ll) {
      if (num <= LLONG_MAX) {
        long long *number = va_arg(*arguments, long long *);
        *number = num * sign;
        status = true;
      }
    } else {
      if (num <= INT_MAX) {
        int *number = va_arg(*arguments, int *);
        *number = num * sign;
        status = true;
      }
    }
  }
  return status;
}

// here we trying to understand what are we dealing with,
// part of int specifier processing
void what_are_we_dealing_with_for_int(const char **str,
                                      enum data_specifiers specifiers,
                                      int *base) {
  if (specifiers == o) {
    *base = 8;
    (*str)++;
  } else if (specifiers == d || specifiers == u) {
    *base = 10;
  } else if (specifiers == x || specifiers == X) {
    *base = 16;
    (*str) = (*str) + 2;
  } else if (specifiers == i) {
    if (**str == '0' && *(*str + 1) == 'x') {
      *base = 16;
      (*str) = (*str) + 2;
    } else if (**str == '0' && **str != 'x') {
      *base = 8;
      (*str)++;
    } else {
      *base = 10;
    }
  }
}

bool is_hex(char a) {
  return (a <= '9' && a >= '0') || (a <= 'f' && a >= 'a') ||
         (a <= 'F' && a >= 'A');
}

// wrote function only for hex nums, but it works good for decimal too :)
int hex_to_int(char a) {
  int num = 0;
  if (a <= '9' && a >= '0') {
    num = a - 48;
  } else if (a <= 'f' && a >= 'a') {
    num = a - 87;
  } else if (a <= 'F' && a >= 'A') {
    num = a - 55;
  }
  return num;
}

bool is_num(char a) { return (a <= '9' && a >= '0'); }

bool is_octal(char a) { return (a <= '7' && a >= '0'); }

int oct_to_int(char a) { return (a - 48); }

// floating point numbers processing part
bool float_specifiers_processing(const char **str,
                                 unsigned int *count_of_success_symbols,
                                 va_list *arguments,
                                 modifiers_struct modifiers) {
  while (**str <= 32) {
    (*str)++;
    (*count_of_success_symbols)++;
  }
  bool status = false;
  long double num = 0;
  int sign = 1;
  if (**str == '-' || **str == '+') {
    sign = (**str == '-') ? -1 : 1;
    (*str)++;
    (*count_of_success_symbols)++;
  }
  while (is_num(**str)) {
    if (status == false && **str == '0') {
      num = 0;
      (*count_of_success_symbols)++;
      (*str)++;
      status = true;
      break;
    }
    status = true;
    num = num * 10 + hex_to_int(**str);
    (*count_of_success_symbols)++;
    (*str)++;
  }
  // here we go if we have point or e
  if (!status && **str == '.' && is_num(*(*str + 1))) {
    status = true;
  }
  if (status && (**str == '.' || **str == 'e' || **str == 'E')) {
    if (**str == '.') {
      (*str)++;
      (*count_of_success_symbols)++;
      num += post_floatingpoint_processing(str, count_of_success_symbols);
      if (**str == 'e' || **str == 'E') {
        (*str)++;
        (*count_of_success_symbols)++;
        status =
            scientific_notation_processing(str, count_of_success_symbols, &num);
      }
    }
  }
  if (**str == 'e' || **str == 'E') {
    (*str)++;
    (*count_of_success_symbols)++;
    status =
        scientific_notation_processing(str, count_of_success_symbols, &num);
  }
  if (status) {
    floatnum_to_argument(num, sign, arguments, modifiers);
  }
  return status;
}

// func just gives the result value to va_arg variable
void floatnum_to_argument(long double result, int sign, va_list *arguments,
                          modifiers_struct modifiers) {
  if (modifiers.suppress) {
    return;
  } else if (modifiers.l) {
    double *number = va_arg(*arguments, double *);
    *number = result * sign;
  } else if (modifiers.ll) {
    long double *number = va_arg(*arguments, long double *);
    *number = result * sign;
  } else {
    float *number = va_arg(*arguments, float *);
    *number = result * sign;
  }
}

// processing numbers after floating point
long double post_floatingpoint_processing(const char **str,
                                          unsigned *count_of_success_symbols) {
  long double result = 0;
  long double count = 0.10000000000;
  while (is_num(**str)) {
    result += hex_to_int(**str) * count;
    (*str)++;
    count = count / 10;
    (*count_of_success_symbols)++;
  }
  return result;
}

// processing scientific notation like e/E and nums after it
bool scientific_notation_processing(const char **str,
                                    unsigned *count_of_success_symbols,
                                    long double *num) {
  bool status = true;
  int mantissa_sign = 1;
  if (**str == '-') {
    mantissa_sign = -1;
    (*count_of_success_symbols)++;
    (*str)++;
  }
  status = false;
  int notation = 0;
  while (is_num(**str)) {
    notation = notation * 10 + hex_to_int(**str);
    (*str)++;
    (*count_of_success_symbols)++;
    status = true;
  }
  *num = *num * pow(10, notation * mantissa_sign);
  return status;
}