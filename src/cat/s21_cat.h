#ifndef S21_CAT_H_
#define S21_CAT_H_

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ShortOption {
    char option;
    int *flag;
};

struct Cats {
    int b;
    int e;
    int n;
    int s;
    int t;
    int v;
    int exitFlag;
};

void processFile(char *argv[], struct Cats *s);
void s21_cat(int b, int e, int n, int s, int t, int v, FILE *file);

#endif  
