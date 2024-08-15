CC = gcc
FLAGS = -Wall -Wextra -Werror -pedantic -std=c11

all: s21_cat style test_func test_leaks clean

s21_cat:
	$(CC) $(FLAGS) s21_cat.c -o s21_cat 

clean: 
	rm s21_cat

rebuild: clean s21_cat

test_func:
	$(CC) $(FLAGS) s21_cat.c -o s21_cat
	sh test_func.sh

test_leaks:
	$(CC) $(FLAGS) s21_cat.c -o s21_cat
	sh test_leak.sh

style:
	clang-format -n s21_cat.c s21_cat.h