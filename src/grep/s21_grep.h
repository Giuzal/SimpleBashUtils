#ifndef s21_GREP_H
#define S21_GREP_H
#define _GNU_SOURCE


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <regex.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>


typedef struct {
  char *pattern;
  int regex_flag;
  bool invert;
  bool count;
  bool filesMatch;
  bool numberLine;
  bool printMatched;
} Flags;

void *xmalloc(size_t size);
void *xrealloc(void *block, size_t size);
Flags GrepReadFlags(int argc, char *argv[]);
void GrepCount(FILE *file, char const *filename, Flags flags, regex_t *preg, int count_file);
void GrepFile(FILE *file, Flags flags, regex_t *preg, char *filename);
void Grep(int argc, char *argv[], Flags flags);


#endif