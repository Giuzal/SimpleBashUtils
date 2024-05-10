#ifndef S21_CAT_H_
#define S21_CAT_H_

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Option {
    char shortOption;
    char *longOption;
    int *flag;
    int b;
    int e;
    int n;
    int s;
    int t;
    int v;
    int exitFlag;
};

void processFile(char *argv[], struct Option *option);
void printFile(int b, int e, int n, int s, int t, int v, FILE *file);
#endif  
