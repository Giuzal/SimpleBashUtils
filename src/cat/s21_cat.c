#include "s21_cat.h"

int main(int argc, char *argv[]) {
    int result = 0;
    struct Option option = {0};
    const struct option longs[] = {
        {"number-nonblank", no_argument, NULL, 'b'},
        {"number", no_argument, NULL, 'n'},
        {"squeeze-blank", no_argument, NULL, 's'},
        {NULL, 0, NULL, 0}
    };

    const char *shorts = "beEstTnv";

    while ((result = getopt_long(argc, argv, shorts, longs, NULL)) != -1) {
        if (result == '?') {
            fprintf(stderr, "Illegal option. Use: cat [-benstuv] [file ...]\n");
            option.exitFlag = 1;
        } else {
            option.shortOption = result;
            switch (result) {
                case 'b':
                    option.b = 1;
                    break;
                case 'e':
                case 'E':
                    option.e = 1;
                    break;
                case 'n':
                    option.n = 1;
                    break;
                case 's':
                    option.s = 1;
                    break;
                case 't':
                case 'T':
                    option.t = 1;
                    break;
                case 'v':
                    option.v = 1;
                    break;
                default:
                    break;
            }
        }
    }

    if (!option.exitFlag) {
        for (int i = optind; i < argc; i++) {
            processFile(argv, &option);
        }
    }

    return 0;
}

void processFile(char *argv[], struct Option *option) {
    FILE *file = NULL;
    if (option->b && option->n) option->n = 0;
    file = fopen(argv[optind], "r+");
    if (!file) {
        fprintf(stderr, "cat: %s: %s\n", argv[optind], strerror(errno));
        fprintf(stderr, "Error opening file.\n");
        option->exitFlag = 1;
    } else {
        printFile(option->b, option->e, option->n, option->s, option->t, option->v, file);
        fclose(file);
    }
}

void printFile(int b, int e, int n, int s, int t, int v, FILE *file) {
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
