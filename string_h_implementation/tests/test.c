#include "test.h"

#include <check.h>
#include <math.h>
#include <string.h>
START_TEST(test_strlen) {
  char *a = "a";
  ck_assert_int_eq(s21_strlen(a), strlen(a));
}
END_TEST

START_TEST(test_strncmp) {
  ck_assert_int_eq(s21_strncmp("", "", 2), strncmp("", "", 2));
  ck_assert_int_eq(s21_strncmp("asdf", "asdf", 2), strncmp("asdf", "asdf", 2));
  ck_assert_int_eq(s21_strncmp("ssdf", "asdf", 2), strncmp("ssdf", "asdf", 2));
  ck_assert_int_eq(s21_strncmp("asfd", "asdf", 2), strncmp("asfd", "asdf", 2));
  ck_assert_int_eq(s21_strncmp("Vasya", "Ilya", 0),
                   strncmp("Vasya", "Ilya", 0));
}
END_TEST

START_TEST(test_memchr) {
  ck_assert_str_eq(s21_memchr("a", (int)'a', 2), memchr("a", (int)'a', 2));
  ck_assert_str_eq(s21_memchr("asdf", (int)'d', 4),
                   memchr("asdf", (int)'d', 4));
  ck_assert_str_eq(s21_memchr("asdf", (int)'a', 2),
                   memchr("asdf", (int)'a', 2));
  ck_assert_str_eq(s21_memchr("asdfz", (int)'z', 5),
                   memchr("asfdz", (int)'z', 5));
  ck_assert_str_eq(s21_memchr("Vasya", (int)'V', 1),
                   memchr("Vasya", (int)'V', 1));
}
END_TEST
START_TEST(test_memcmp) {
  int n = 3;
  ck_assert_int_eq(s21_memcmp("", "", 3), memcmp("", "", 3));
  ck_assert_int_eq(s21_memcmp("asdf", "asdf", 3), memcmp("asdf", "asdf", 3));
  ck_assert_int_eq(s21_memcmp("asdf", "xsdf", n), memcmp("asdf", "xsdf", n));
  ck_assert_int_eq(s21_memcmp("asfd", "asdx", n), memcmp("asfx", "asdx", n));
  ck_assert_int_eq(s21_memcmp("Vasya", "Ilya", 0), memcmp("Vasya", "Ilya", 0));
}
END_TEST
START_TEST(test_memcpy) {
  char a[] = "\0\0\0\0\0\0";
  char b[] = "\0\0\0\0\0\0";
  ck_assert_str_eq(s21_memcpy(a, "asdf", 4), memcpy(b, "asdf", 4));
  ck_assert_str_eq(a, b);
  char c[] = "\0\0\0\0\0\0";
  char d[] = "\0\0\0\0\0\0";
  ck_assert_str_eq(s21_memcpy(c, "asdf", 2), memcpy(d, "asdf", 2));
  ck_assert_str_eq(c, d);
  char e[] = "\0\0\0\0\0\0";
  char f[] = "\0\0\0\0\0\0";
  ck_assert_str_eq(s21_memcpy(e, "", 1), memcpy(f, "", 1));
  ck_assert_str_eq(e, f);
  char g[] = "\0\0\0\0\0\0";
  char h[] = "\0\0\0\0\0\0";
  ck_assert_str_eq(s21_memcpy(g, "fasdf", 5), memcpy(h, "fasdf", 5));
  ck_assert_str_eq(g, h);
  char i[] = "\0\0\0\0\0\0";
  char j[] = "\0\0\0\0\0\0";
  ck_assert_str_eq(s21_memcpy(i, "asdf", 0), memcpy(j, "asdf", 0));
  ck_assert_str_eq(i, j);
}
END_TEST
START_TEST(test_memset) {
  char a[] = "zzzzzzzzzz";
  char b[] = "zzzzzzzzzz";
  char l = 'a';
  int n = 10;
  ck_assert_str_eq(s21_memset(a, l, n), memset(b, l, n));
  char c[] = "aaaaaaaaaaa";
  char d[] = "aaaaaaaaaaa";
  ck_assert_str_eq(s21_memset(c, (int)'s', 10), memset(d, (int)'s', 10));

  char e[] = "-------------";
  char f[] = "-------------";
  ck_assert_str_eq(s21_memset(e, (int)' ', 10), memset(f, (int)' ', 10));
  ck_assert_str_eq(e, f);
  char g[] = "sdfgsd";
  char h[] = "sdfgsd";
  ck_assert_str_eq(s21_memset(g, (int)'a', 1), memset(h, (int)'a', 1));
  ck_assert_str_eq(g, h);
}
END_TEST
START_TEST(test_strchr) {
  ck_assert_ptr_eq(s21_strchr("asdf", 'm'), s21_NULL);
  ck_assert_str_eq(s21_strchr("asdf", 'd'), strchr("asdf", 'd'));
  ck_assert_str_eq(s21_strchr("asdf", '\0'), strchr("asdf", '\0'));
}
END_TEST

START_TEST(test_strcspn) {
  ck_assert_int_eq(s21_strcspn("Danila", "lai"), s21_strcspn("Danila", "lai"));
  ck_assert_int_eq(s21_strcspn("Danila", "xyz"), s21_strcspn("Danila", "xyz"));
  ck_assert_int_eq(s21_strcspn("Danila", ""), s21_strcspn("Danila", ""));
}
END_TEST
START_TEST(test_strerror) {
  ck_assert_str_eq(s21_strerror(1), strerror(1));
  ck_assert_str_eq(s21_strerror(12), strerror(12));
  ck_assert_str_eq(s21_strerror(4), strerror(4));
}
END_TEST
START_TEST(test_to_lower) {
  char a[] = "ABCD3";
  char *d = (char *)s21_to_lower(a);
  ck_assert_str_eq(d, "abcd3");
  char b[] = "12345";
  char *e = (char *)s21_to_lower(b);
  ck_assert_str_eq(e, "12345");
  char c[] = "a2123CCs";
  char *f = (char *)s21_to_lower(c);
  ck_assert_str_eq(f, "a2123ccs");
  free(d);
  free(e);
  free(f);
}
START_TEST(test_to_upper) {
  char a[] = "abcd3";
  char *d = (char *)s21_to_upper(a);
  ck_assert_str_eq(d, "ABCD3");
  char b[] = "12345";
  char *e = (char *)s21_to_upper(b);
  ck_assert_str_eq(e, "12345");
  char c[] = "A2123ccS";
  char *f = (char *)s21_to_upper(c);
  ck_assert_str_eq(f, "A2123CCS");
  free(d);
  free(e);
  free(f);
}
START_TEST(test_trim) {
  char a[] = " HelloWorld!   ";
  char *d = (char *)s21_trim(a, s21_NULL);
  ck_assert_str_eq(d, "HelloWorld!");
  char b[] = "12345";
  char *e = (char *)s21_trim(b, " ");
  ck_assert_str_eq(e, "12345");
  char c[] = "A2123ccS.";
  char *f = (char *)s21_trim(c, ".");
  ck_assert_str_eq(f, "A2123ccS");
  free(d);
  free(e);
  free(f);
}
START_TEST(test_insert) {
  char a[20] = "Hello!";
  char *b = s21_insert(a, " World", 5);
  ck_assert_str_eq(b, "Hello World!");
  char *c = s21_insert(a, "", 5);
  ck_assert_str_eq(c, "Hello!");
  char *d = s21_insert(a, s21_NULL, 5);
  ck_assert_ptr_null(d);
  if (b) free(b);
  if (c) free(c);
  if (d) free(d);
}
START_TEST(test_strncat) {
  char a[6] = "Sl";
  char b[6] = "Sl";
  ck_assert_str_eq(s21_strncat(a, "ovo", 2), strncat(b, "ovo", 2));
  char c[9] = "Sen";
  char d[9] = "Sen";
  ck_assert_str_eq(s21_strncat(c, "tence", 3), strncat(d, "tence", 3));
  char e[4] = "Sen";
  char f[4] = "Sen";
  ck_assert_str_eq(s21_strncat(e, "tence", 0), strncat(f, "tence", 0));
}
START_TEST(test_strpbrk) {
  ck_assert_str_eq(s21_strpbrk("0123456789", "369"),
                   strpbrk("0123456789", "369"));
  ck_assert_ptr_eq(s21_strpbrk("0123456789", "abcd"),
                   strpbrk("0123456789", "abcd"));
  ck_assert_str_eq(s21_strpbrk("Lorem ipsum", " "),
                   strpbrk("Lorem ipsum", " "));
  ck_assert_str_eq(s21_strpbrk("Lorem ipsum", "m"),
                   strpbrk("Lorem ipsum", "m"));
}
END_TEST

START_TEST(test_strrchr) {
  ck_assert_str_eq(s21_strrchr("abcba\0", 'b'), strrchr("abcba\0", 'b'));
  ck_assert_ptr_eq(s21_strrchr("abcba\0", 'k'), strrchr("abcba\0", 'k'));
  ck_assert_str_eq(s21_strrchr("abcdcba\0", 'a'), strrchr("abcdcba\0", 'a'));
}
START_TEST(test_strstr) {
  ck_assert_str_eq(s21_strstr("Sentence\0", "enc"),
                   strstr("Sentence\0", "enc"));
  ck_assert_ptr_eq(s21_strstr("Sentence\0", "Word"),
                   strstr("Sentence\0", "Word"));
  ck_assert_str_eq(s21_strstr("Sentence\0", "Sentence"),
                   strstr("Sentence\0", "Sentence"));
  ck_assert_str_eq(s21_strstr("Sentence\0", "\0"), strstr("Sentence\0", "\0"));
}
START_TEST(test_strncpy) {
  char a[15];
  char b[15];
  ck_assert_str_eq(s21_strncpy(a, "enc\0", 4), strncpy(b, "enc\0", 4));
  char c[15];
  char d[15];
  ck_assert_str_eq(s21_strncpy(c, "Word\0", 5), strncpy(d, "Word\0", 5));
  char e[15];
  char f[15];
  ck_assert_str_eq(s21_strncpy(e, "Sentence\0", 9),
                   strncpy(f, "Sentence\0", 9));
}
START_TEST(test_strtok) {
  char a[] = "-.-Lorem ipsum dolor-/sit amet, consectetur/ adipiscing elit.";
  char b[] = "-.-Lorem ipsum dolor-/sit amet, consectetur/ adipiscing elit.";
  ck_assert_str_eq(s21_strtok(a, ".-/,"), strtok(b, ".-/,"));
  ck_assert_str_eq(s21_strtok(s21_NULL, ".-/,"), strtok(s21_NULL, ".-/,"));
  ck_assert_str_eq(s21_strtok(s21_NULL, ".-/,"), strtok(s21_NULL, ".-/,"));
  ck_assert_str_eq(s21_strtok(s21_NULL, ".-/,"), strtok(s21_NULL, ".-/,"));
}
END_TEST
START_TEST(test_sscanf) {
  ck_assert_int_eq(s21_sscanf(s21_NULL, s21_NULL), -1);
  char letter = '9';
  ck_assert_int_eq(s21_sscanf("?", "%c", &letter), sscanf("?", "%c", &letter));
  char string[20] = "NOHELLO";
  long num_of_chars;
  ck_assert_int_eq(s21_sscanf("hello_world", "%s%ln", string, &num_of_chars),
                   sscanf("hello_world", "%s%ln", string, &num_of_chars));
  int number = 0;
  ck_assert_int_eq(s21_sscanf("123", "%d", &number),
                   sscanf("123", "%d", &number));
  ck_assert_int_eq(s21_sscanf("123", "%n", &number),
                   sscanf("123", "%n", &number));
  unsigned unum = 0;
  ck_assert_int_eq(s21_sscanf("0x52af", "%x", &unum),
                   sscanf("0x52af", "%x", &unum));
  long unsigned int lunum = 0;

  ck_assert_int_eq(s21_sscanf("0x51afdc", "%lx", &lunum),
                   sscanf("0x51afdc", "%lx", &lunum));
  ck_assert_int_eq(s21_sscanf("0561", "%lo", &lunum),
                   sscanf("0561", "%lo", &lunum));
  ck_assert_int_eq(s21_sscanf("0x51afdc 0x51afdc", "%*x %lx", &lunum),
                   sscanf("0x51afdc 0x51afdc", "%*x %lx", &lunum));
  void *pointer;
  ck_assert_int_eq(s21_sscanf("0x7f1908fbd060", "%p", &pointer),
                   sscanf("0x7f1908fbd060", "%p", &pointer));
  double dbl;
  ck_assert_int_eq(s21_sscanf("0.56E2", "%le", &dbl),
                   sscanf("0.56E2", "%le", &dbl));
  ck_assert_int_eq(s21_sscanf("0.56E2", "%lE", &dbl),
                   sscanf("0.56E2", "%lE", &dbl));
  ck_assert_int_eq(s21_sscanf("0.5632112", "%lf", &dbl),
                   sscanf("0.5632112", "%lf", &dbl));
  ck_assert_int_eq(s21_sscanf("0.5632112", "%lg", &dbl),
                   sscanf("0.5632112", "%lg", &dbl));
  short hnum;
  ck_assert_int_eq(s21_sscanf("-51", "%hi", &hnum),
                   sscanf("-51", "%hi", &hnum));

  char str[30] = "no hello";
  char litera = '!';
  int num = 228, count_letters;
  unsigned hex_num = 0xAAA, octal_num = 0777;
  long long_num = 228;
  float just_float = 0.228;
  double just_lfloat = 0.228228, science_num = 228E-1;
  ck_assert_int_eq(
      s21_sscanf("Hello_world 132 0XFC32B 054714 -2147483000 0.314314? "
                 "0.314314314314314 314E-5",
                 "%s %d %x %o %ld %Lf%c%lf%lE %n", str, &num, &hex_num,
                 &octal_num, &long_num, &just_float, &litera, &just_lfloat,
                 &science_num, &count_letters),
      sscanf("Hello_world 132 0XFC32B 054714 -2147483000 0.314314? "
             "0.314314314314314 314E-5",
             "%s %d %x %o %ld %f%c%lf%lE %n", str, &num, &hex_num, &octal_num,
             &long_num, &just_float, &litera, &just_lfloat, &science_num,
             &count_letters));

  int *a1 = 0, *a2 = 0, *b1 = 0, *b2 = 0, *c1 = 0, *c2 = 0, *d1 = 0, *d2 = 0;

  const char *format = "%p %p %p %p";
  const char *str1 = "0xFFFF 0xAAAA 0x7123 0xBBC1FF";

  int16_t res1 = s21_sscanf(str1, format, &a1, &b1, &c1, &d1);
  int16_t res2 = sscanf(str1, format, &a2, &b2, &c2, &d2);

  ck_assert_ptr_eq(a1, a2);
  ck_assert_ptr_eq(b1, b2);
  ck_assert_ptr_eq(c1, c2);
  ck_assert_ptr_eq(d1, d2);
  ck_assert_int_eq(res1, res2);

  ck_assert_int_eq(s21_sscanf("0x51afdc", "hi%lx", &lunum),
                   sscanf("0x51afdc", "hi%lx", &lunum));
}
END_TEST
START_TEST(test_sprintf) {
  char a[10000];
  char b[10000];
  long int c = 241231;
  long long unsigned d = 12353124124564;
  int e = 12423;
  float f = 23.52;
  long double g = -0.00203L;
  long long h = 12413545L;
  long unsigned int i = 11114212;
  char str[] =
      "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque "
      "eget lacus nec felis luctus ultrices non at mi. Interdum et malesuada "
      "fames ac ante ipsum primis in faucibus. Sed nec molestie est, vitae "
      "lacinia nisi.";
  wchar_t str1[] =
      L"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque "
      "eget lacus nec felis luctus ultrices non at mi. Interdum et malesuada "
      "fames ac ante ipsum primis in faucibus. Sed nec molestie est, vitae "
      "lacinia nisi.";
  wchar_t j = L'ф';
  ck_assert_int_eq(s21_sprintf(a, "%d", e), sprintf(b, "%d", e));
  ck_assert_int_eq(s21_sprintf(a, "%+13.3ld", c), sprintf(b, "%+13.3ld", c));
  ck_assert_int_eq(s21_sprintf(a, "% *.3lld", 13, h),
                   sprintf(b, "% *.3lld", 13, h));
  ck_assert_str_eq(a, b);
  ck_assert_int_eq(s21_sprintf(a, "%#13o", e), sprintf(b, "%#13o", e));
  ck_assert_str_eq(a, b);
  ck_assert_int_eq(s21_sprintf(a, "%.13lu %%", i), sprintf(b, "%.13lu %%", i));
  ck_assert_int_eq(s21_sprintf(a, "%.13llu %%", d),
                   sprintf(b, "%.13llu %%", d));
  ck_assert_str_eq(a, b);
  ck_assert_int_eq(s21_sprintf(a, "%p %.3f %g %.2e", a, f, f, f),
                   sprintf(b, "%p %.3f %g %.2e", a, f, f, f));
  ck_assert_str_eq(a, b);
  ck_assert_int_eq(s21_sprintf(a, "%Lf %10Lg %-3Le", g, g, g),
                   sprintf(b, "%Lf %10Lg %-3Le", g, g, g));
  ck_assert_str_eq(a, b);
  ck_assert_int_eq(s21_sprintf(a, "%13c", 'a'), sprintf(b, "%13c", 'a'));
  ck_assert_str_eq(a, b);
  ck_assert_int_eq(s21_sprintf(a, "%13lc", j), sprintf(b, "%13lc", j));
  ck_assert_str_eq(a, b);
  ck_assert_int_eq(s21_sprintf(a, "%13s", str), sprintf(b, "%13s", str));
  ck_assert_str_eq(a, b);
  ck_assert_int_eq(s21_sprintf(a, "%13ls", str1), sprintf(b, "%13ls", str1));
  ck_assert_str_eq(a, b);
}
END_TEST

Suite *string_suite() {
  Suite *suite;
  TCase *tc_strlen, *tc_strncmp, *tc_memchr, *tc_memcmp, *tc_memcpy, *tc_memset,
      *tc_strchr, *tc_strcspn, *tc_strerror, *tc_strncat, *tc_strpbrk,
      *tc_strrchr, *tc_strstr, *tc_strtok, *tc_strncpy, *tc_to_lower,
      *tc_to_upper, *tc_trim, *tc_insert, *tc_sscanf, *tc_sprintf;
  suite = suite_create("s21_string");

  tc_strlen = tcase_create("s21_strlen");
  tcase_add_test(tc_strlen, test_strlen);
  suite_add_tcase(suite, tc_strlen);

  tc_strncmp = tcase_create("s21_strncmp");
  tcase_add_test(tc_strncmp, test_strncmp);
  suite_add_tcase(suite, tc_strncmp);

  tc_memchr = tcase_create("s21_memchr");
  tcase_add_test(tc_memchr, test_memchr);
  suite_add_tcase(suite, tc_memchr);

  tc_memcmp = tcase_create("s21_memcmp");
  tcase_add_test(tc_memcmp, test_memcmp);
  suite_add_tcase(suite, tc_memcmp);

  tc_memcpy = tcase_create("s21_memcpy");
  tcase_add_test(tc_memcpy, test_memcpy);
  suite_add_tcase(suite, tc_memcpy);

  tc_memset = tcase_create("s21_memset");
  tcase_add_test(tc_memset, test_memset);
  suite_add_tcase(suite, tc_memset);

  tc_strchr = tcase_create("s21_strchr");
  tcase_add_test(tc_strchr, test_strchr);
  suite_add_tcase(suite, tc_strchr);

  tc_strcspn = tcase_create("s21_strcspn");
  tcase_add_test(tc_strcspn, test_strcspn);
  suite_add_tcase(suite, tc_strcspn);

  tc_strerror = tcase_create("s21_strerror");
  tcase_add_test(tc_strerror, test_strerror);
  suite_add_tcase(suite, tc_strerror);

  tc_strncat = tcase_create("s21_strncat");
  tcase_add_test(tc_strncat, test_strncat);
  suite_add_tcase(suite, tc_strncat);

  tc_strpbrk = tcase_create("s21_strpbrk");
  tcase_add_test(tc_strpbrk, test_strpbrk);
  suite_add_tcase(suite, tc_strpbrk);

  tc_strrchr = tcase_create("s21_strrchr");
  tcase_add_test(tc_strrchr, test_strrchr);
  suite_add_tcase(suite, tc_strrchr);

  tc_strstr = tcase_create("s21_strstr");
  tcase_add_test(tc_strstr, test_strstr);
  suite_add_tcase(suite, tc_strstr);

  tc_strncpy = tcase_create("s21_strncpy");
  tcase_add_test(tc_strncpy, test_strncpy);
  suite_add_tcase(suite, tc_strncpy);

  tc_strtok = tcase_create("s21_strtok");
  tcase_add_test(tc_strtok, test_strtok);
  suite_add_tcase(suite, tc_strtok);

  tc_to_lower = tcase_create("s21_to_lower");
  tcase_add_test(tc_to_lower, test_to_lower);
  suite_add_tcase(suite, tc_to_lower);

  tc_to_upper = tcase_create("s21_to_upper");
  tcase_add_test(tc_to_upper, test_to_upper);
  suite_add_tcase(suite, tc_to_upper);

  tc_trim = tcase_create("s21_trim");
  tcase_add_test(tc_trim, test_trim);
  suite_add_tcase(suite, tc_trim);

  tc_insert = tcase_create("s21_insert");
  tcase_add_test(tc_insert, test_insert);
  suite_add_tcase(suite, tc_insert);

  tc_sscanf = tcase_create("s21_sscanf");
  tcase_add_test(tc_sscanf, test_sscanf);
  suite_add_tcase(suite, tc_sscanf);

  tc_sprintf = tcase_create("s21_sprintf");
  tcase_add_test(tc_sprintf, test_sprintf);
  suite_add_tcase(suite, tc_sprintf);

  return suite;
}