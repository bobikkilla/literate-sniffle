s21_grep:
	gcc -Wall -Wextra -Werror -std=c11 s21_grep.c -o s21_grep
clean:
	rm -rf s21_grep

rebuild: clean s21_grep

test:
	clang-format -n *.c *.h
	sleep 3s
	./tests.sh
	sleep 3s
	./test_leaks.sh

leaks:
	./test_leaks.sh