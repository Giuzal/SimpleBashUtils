#ifndef s21_GREP_H
#define S21_GREP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <regex.h>
#include <errno.h>


Flags GrepReadFlags(int argc, char *argv[]);
void GrepCount(FILE *file, char const *filename, Flags flags, regex_t *preg);
void GrepFile(FILE *file, Flags flags, regex_t *preg);
void Grep(int argc, char *argv[], Flags flags);


#endif  
