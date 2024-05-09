#include "s21_cat.h"

int main(int argc, char *argv[]) {
    int result = 0;
    struct Cats s = {0};
    const char *shorts = "beEstTnv";
    const struct option longs[] = {
        {"number-nonblank", no_argument, NULL, 'b'},
        {"number", no_argument, NULL, 'n'},
        {"squeeze-blank", no_argument, NULL, 's'},
        {NULL, 0, NULL, 0}
    };

    struct ShortOption shortOpts[] = {
        {'b', &s.b},
        {'e', &s.e},
        {'E', &s.e},
        {'n', &s.n},
        {'s', &s.s},
        {'t', &s.t},
        {'T', &s.t},
        {'v', &s.v}
    };

    while ((result = getopt_long(argc, argv, shorts, longs, NULL)) != -1) {
        size_t i;
        for (i = 0; i < sizeof(shortOpts) / sizeof(shortOpts[0]); i++) {
            if (result == shortOpts[i].option) {
                *(shortOpts[i].flag) = 1;
                break;
            }
        }
        if (result == '?') {
            fprintf(stderr, "Illegal option. Use: cat [-benstuv] [file ...]\n");
            s.exitFlag = 1;
        }
    }

    if (!s.exitFlag) {
        for (int i = optind; i < argc; i++) {
            processFile(argv, &s);
        }
    }

    return 0;
}

void processFile(char *argv[], struct Cats *s) {
    FILE *file = NULL;
    if (s->b && s->n) s->n = 0;
    file = fopen(argv[optind], "r+");
    if (!file) {
        fprintf(stderr, "cat: %s: %s\n", argv[optind], strerror(errno));
        fprintf(stderr, "Error opening file.\n");
        s->exitFlag = 1;
    } else {
        s21_cat(s->b, s->e, s->n, s->s, s->t, s->v, file);
        fclose(file);
    }
}

void s21_cat(int b, int e, int n, int s, int t, int v, FILE *file) {
    int ch, prev_ch = 1, number_of_empty = 0, lineCount = 1, firstLine = 1;
    while ((ch = fgetc(file)) != EOF) {
        if (s) {
            if (ch == '\n') number_of_empty++;
            if (ch != '\n') number_of_empty = 0;
        }
        if (ch == '\n' && (!s || number_of_empty < 3)) {
            if ((firstLine == 1 && n) || (n && prev_ch == '\n')) printf("%6d\t", lineCount++);
            if (e) {
                if ((prev_ch == '\n' || firstLine == 1) && b) printf("      \t");
                printf("$");
            }
            printf("%c", ch);
        }
        if (ch != '\n') {
            if ((prev_ch == '\n' || firstLine == 1) && (n || b)) printf("%6d\t", lineCount++);
            if (ch < 32 && ch != 9 && ch != 10 && v)
                printf("^%c", ch + 64);
            else if (ch > 127 && ch < 160 && v)
                printf("M-^%c", ch - 64);
            else if (ch == 127 && v)
                printf("^%c", ch - 64);
            else if (ch == '\t' && t)
                printf("^I");
            else
                printf("%c", ch);
        }
        prev_ch = ch;
        firstLine = 0;
    }
}
