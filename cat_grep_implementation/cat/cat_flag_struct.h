#ifndef FLAG_STRUCT
#define FLAG_STRUCT

typedef struct cat_flags {
  int flag_T;  // если ввели маленькую букву, включаем Tv; /t = ^I
  int flag_E;  // если ввели маленькую букву, включаем Ev; /n = $
  int flag_b;  // --number-nonblank; нумерует только непустые строки
  int flag_n;  // --number;          нумерует все выходные строки
  int flag_s;  // --squeeze-blank;   сжимает несколько смежных пустых строк
  int flag_v;  // что это
} cat_flags;

#endif