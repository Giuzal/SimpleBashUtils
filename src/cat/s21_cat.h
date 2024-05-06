#ifndef S21_CAT_H
#define S21_CAT_H

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>

typedef struct {
    int number_lines;
    int number_nonempty;
    int show_tabs;
    int show_ends;
    int show_nonprinting;
    int squeeze_blank;
} Flags;

void CatNoArgs();
#endif
