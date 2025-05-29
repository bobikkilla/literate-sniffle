#ifndef STRING_H
#define STRING_H

/*This macro is the value of a null pointer constant. */
#define s21_NULL ((void *)0)

/*This is the unsigned integral type and is the result of the sizeof keyword. */
typedef long unsigned s21_size_t;

#include <limits.h>
#include <locale.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

/*string.h functions*/

/*Searches for the first occurrence of the character c (an unsigned char) in the
 * first n bytes of the string pointed to, by the argument str. */
void *s21_memchr(const void *str, int c, s21_size_t n);

/*Compares the first n bytes of str1 and str2. */
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);

/*Copies n characters from src to dest. */
void *s21_memcpy(void *dest, const void *src, s21_size_t n);

/*Copies the character c (an unsigned char) to the first n characters of the
 * string pointed to, by the argument str.*/
void *s21_memset(void *str, int c, s21_size_t n);

/*Appends the string pointed to, by src to the end of the string pointed to, by
 * dest up to n characters long. */
char *s21_strncat(char *dest, const char *src, s21_size_t num);

/*Searches for the first occurrence of the character c (an unsigned char) in the
 * string pointed to, by the argument str. */
char *s21_strchr(const char *str, int c);

/*Compares at most the first n bytes of str1 and str2. */
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);

/*Copies up to n characters from the string pointed to, by src to dest. */
char *s21_strncpy(char *dest, const char *src, s21_size_t n);

/*Calculates the length of the initial segment of str1 which consists entirely
 * of characters not in str2. */
s21_size_t s21_strcspn(const char *str1, const char *str2);

/*Searches an internal array for the error number errnum and returns a pointer
 * to an error message string. You need to declare macros containing arrays of
 * error messages for mac and linux operating systems. Error descriptions are
 * available in the original library. Checking the current OS is carried out
 * using directives. */
char *s21_strerror(int errnum);

/*Computes the length of the string str up to but not including the terminating
null character.
--NULL leads to segfault, but original string.h behave the same*/
s21_size_t s21_strlen(const char *str);

/* Finds the first character in the string str1 that matches any character
 * specified in str2. */
char *s21_strpbrk(const char *str1, const char *str2);

/*Searches for the last occurrence of the character c (an unsigned char) in the
 * string pointed to by the argument str.*/
char *s21_strrchr(const char *str, int c);

/*Finds the first occurrence of the entire string needle (not including the
 * terminating null character) which appears in the string haystack.*/
char *s21_strstr(const char *haystack, const char *needle);

/*Breaks string str into a series of tokens separated by delim.*/
char *s21_strtok(char *str, const char *delim);

/*hard functions*/

/*reads formatted input from a string*/
int s21_sscanf(const char *str, const char *format, ...);

/*sends formatted output to a string pointed to, by str*/
int s21_sprintf(char *str, const char *format, ...);

/* bonus functions */

/*Returns a copy of string (str) converted to uppercase. In case of any error,
 * return NULL.*/
void *s21_to_upper(const char *str);

/*Returns a copy of string (str) converted to lowercase. In case of any error,
 * return NULL.*/
void *s21_to_lower(const char *str);

/*Returns a new string in which a specified string (str) is inserted at a
   specified index position (start_index) in the given string (src). In case of
   any error, return NULL.*/
void *s21_insert(const char *src, const char *str, size_t start_index);

/*Returns a new string in which all leading and trailing occurrences of a set of
 * specified characters (trim_chars) from the given string (src) are removed. In
 * case of any error, return NULL.
 */
void *s21_trim(const char *src, const char *trim_chars);

#endif
