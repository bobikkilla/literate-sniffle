s21_cat:
	gcc -Wall -Wextra -Werror -std=c11 s21_cat.c -o s21_cat

rebuild:
	rm -rf s21_cat
	gcc -Wall -Wextra -Werror -std=c11 s21_cat.c -o s21_cat
clean:
	rm -rf s21_cat
test:
	clang-format -n *.c *.h
	sleep 3s
	./tests.sh
	sleep 3s
	./test_leaks.sh