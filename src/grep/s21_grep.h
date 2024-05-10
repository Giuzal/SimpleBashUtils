#ifndef s21_GREP_H
#define S21_GREP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <regex.h>
#include <errno.h>
#include <unistd.h>


typedef struct {
  int regex_flag;
  bool invert;
  bool count;
  bool filesMatch;
  bool numberLine;
  bool printMatched;
} Flags;



Flags GrepReadFlags(int argc, char *argv[]);
void GrepCount(FILE *file, char const *filename, Flags flags, regex_t *preg, int count_file);
void GrepFile(FILE *file, Flags flags, regex_t *preg, char *filename);
void Grep(int argc, char *argv[], Flags flags);


#endif  
