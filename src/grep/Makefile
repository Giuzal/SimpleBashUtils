CC = gcc
FLAGS = -Wall -Wextra -Werror

all: clean s21_grep

s21_grep: Makefile s21_grep.h s21_grep.c
	$(CC) $(FLAGS) s21_grep.c -o s21_grep

rebuild:
	make clean
	make all

clean:
	rm -rf s21_grep

#test: s21_cat test_script.sh
#./test_script.sh

.PHONY: all clean test
