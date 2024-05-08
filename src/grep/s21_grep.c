#include "s21_grep.h"

typedef struct {
  int regex_flag;
  bool invert;
  bool count;
  bool filesMatch;
  bool numberLine;
} Flags;

Flags GrepReadFlags(int argc, char *argv[]) {
  Flags flags = {0, false, false, false, false};
  int currentFlag = getopt(argc, argv, "eivcln");
  for (; currentFlag != -1; currentFlag = getopt(argc, argv, "eivcln")) {
    switch (currentFlag) {
      case 'e':
        flags.regex_flag |= REG_EXTENDED;
        break;
      case 'i':
        flags.regex_flag |= REG_ICASE;
        break;
      case 'v':
        flags.invert |= true;
        break;
      case 'c':
        flags.count = true;
        break;
      case 'l':
        flags.filesMatch = true;
        break;
      case 'n':
        flags.numberLine = true;
        /* break; case 'o':
            flags. = true;
     */
        /*  default:
             break; */
    }
  }
  return flags;
}

void GrepCount(FILE *file, char const *filename, Flags flags, regex_t *preg) {
  (void)flags;
  (void)filename;
  char *line = 0;
  size_t length = 0;
  regmatch_t match;
  int count = 0;
  while (getline(&line, &length, file) > 0) {
    if (!regexec(preg, line, 1, &match, 0)) {
      ++count;
    }
  }
  printf("%i\n", count);
  free(line);
}

void GrepFile(FILE *file, Flags flags, regex_t *preg) {
  (void)flags;
  char *line = 0;
  size_t length = 0;
  regmatch_t match;
  while (getline(&line, &length, file) > 0) {
    if (flags.invert) {
      if (regexec(preg, line, 1, &match, 0)) {
        printf("%s", line);
      }
    } else {
      if (!regexec(preg, line, 1, &match, 0)) {
        printf("%s", line);
      }
    }
  }
  free(line);
}

void Grep(int argc, char *argv[], Flags flags) {
  char **pattern = &argv[1];
  char **end = &argv[argc];
  regex_t preg_storage;
  regex_t *preg = &preg_storage;
  for (;pattern != end && pattern[0][0] == '-'; ++pattern)
    ;
  if (pattern == end) {
    fprintf(stderr, "Enter valued pattern\n");
    exit(1);
  }
  if (regcomp(preg, *pattern, flags.regex_flag)) {
    fprintf(stderr, "cimpiling error\n");
    exit(1);
  }
  for (char **filename = pattern + 1; filename != end; ++filename) {
    if (**filename == '-') continue;
    FILE *file = fopen(*filename, "rb");
    if (errno) {
      fprintf(stderr, "%s", argv[0]);
      perror(*filename);
      continue;
    }
    if (flags.count) {
      GrepCount(file, *filename, flags, preg);
    } else
      GrepFile(file, flags, preg);
    fclose(file);
  }
}

int main(int argc, char *argv[]) {
  Flags flags = GrepReadFlags(argc, argv);
  Grep(argc, argv, flags);
}
