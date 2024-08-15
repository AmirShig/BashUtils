CC = gcc
FLAGS = -Wall -Wextra -Werror -pedantic -std=c11

all: s21_grep style test_func test_leaks clean

s21_grep:
	$(CC) $(FLAGS) s21_grep.c -o s21_grep 

clean:
	rm s21_grep

rebuild: clean s21_grep

test_func:
	$(CC) $(FLAGS) s21_grep.c -o s21_grep
	sh func_test_grep.sh

test_leaks:
	$(CC) $(FLAGS) s21_grep.c -o s21_grep
	sh leaks_test_grep.sh

style:
	clang-format -n s21_grep.c s21_grep.h