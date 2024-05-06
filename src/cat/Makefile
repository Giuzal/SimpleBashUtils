CC = gcc
FLAGS = -Wall -Wextra -Werror

all: clean s21_cat

s21_cat: Makefile s21_cat.h s21_cat.c
	$(CC) $(FLAGS) s21_cat.c -o s21_cat

rebuild:
	make clean
	make all

clean:
	rm -rf s21_cat

test: s21_cat test_script.sh
	./test_script.sh

.PHONY: all clean test

