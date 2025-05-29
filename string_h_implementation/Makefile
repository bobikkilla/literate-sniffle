CC = gcc
TFLAGS = -Werror -Wall -Wextra -std=c11 
OFLAGS = -c -Werror -Wall -Wextra -std=c11 
TEST = ./tests/main.c ./tests/test.c
AR = ar -rc 
VAL = valgrind --tool=memcheck --leak-check=yes
LIBS = -lcheck -lsubunit -lgcov -lm
OBJ = s21_insert.o s21_to_lower.o s21_to_upper.o s21_trim.o s21_memchr.o s21_memcmp.o s21_memcpy.o s21_memset.o s21_sprintf.o s21_sscanf.o s21_strchr.o s21_strcspn.o s21_strerror.o s21_strlen.o s21_strncat.o s21_strncmp.o s21_strncpy.o s21_strpbrk.o s21_strrchr.o s21_strstr.o s21_strtok.o
all: s21_string.a
s21_string.a: $(OBJ)
	$(AR) $@ $^
%.o: %.c
	$(CC) $(OFLAGS) $< -o $@
gcov_report: OFLAGS = -Wall -Werror -Wextra -std=c11 -c --coverage -fprofile-arcs -ftest-coverage 
gcov_report: test
	./$<
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory out
test: $(OBJ)
	$(CC) $(TFLAGS) $(TEST) $^ -o $@ $(LIBS)
	./$@
	$(VAL) ./$@
.PHONY: clean

clean:
	-rm *.o s21_string.a *.gcda *.gcno test coverage.info 
	-rm -r out