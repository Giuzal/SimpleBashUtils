#include "s21_grep.h"


//-l, -h, -s, -e флаг дописать, Обработку ошибок согласно реальному грепу
// убрать все глобальные переменные:errno

void *xmalloc(size_t size){
  void *temp;
  temp = malloc(size);
  if (!temp)
    exit(errno);
  return temp;
}

void *xrealloc(void *block, size_t size){
  void *temp;
  temp = realloc(block, size);
  if (!temp)
    exit(errno);
  return temp;
}

Flags GrepReadFlags(int argc, char *argv[]) {
  Flags flags = {0, false, false, false, false, false};
  int currentFlag = getopt(argc, argv, "eivclno");
  flags.pattern = xmalloc(2);
  flags.pattern[0] = '[';
  flags.pattern[1] = '\0';
  size_t pattern_size = 1;
  size_t temp_size = 0;
  for (; currentFlag != -1; currentFlag = getopt(argc, argv, "eivclno")) {
    switch (currentFlag) { 
      case 'e':
        temp_size = strlen(optarg);
        flags.pattern = xrealloc(flags.pattern, pattern_size + temp_size + 3);
        flags.pattern[pattern_size] = '(';
        ++pattern_size;
        memcpy(flags.pattern + pattern_size + 1, optarg, temp_size);
        pattern_size += temp_size;
        flags.pattern[pattern_size] = ')';
        ++pattern_size;
        flags.pattern[pattern_size] = '\0';
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
      break; case 'o':
        flags.printMatched = true;
        /*  default:
             break; */
    }
  }
  return flags;
}

void GrepCount(FILE *file, char const *filename, Flags flags, regex_t *preg, int count_file) {
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
  if (count_file == 1)
    printf("%i\n", count);
  else 
    printf("%s:%i\n", filename, count);
  free(line);
}

void GrepFile(FILE *file, Flags flags, regex_t *preg, char *filename) {
  (void)flags;
  char *line = 0;
  size_t length = 0;
  regmatch_t match;
  int count = 0;
  while (getline(&line, &length, file) > 0) {
    ++count;
    if (flags.invert) {
      if (regexec(preg, line, 1, &match, 0)) {
        if (flags.printMatched);
          else {
            if (flags.numberLine)
              printf("%s:%i:%s", filename, count, line);
            else
              printf("%s", line);
          }
      }
    } else {
      if (!regexec(preg, line, 1, &match, 0)) {
        if (flags.printMatched){
           if (flags.numberLine)
              printf("%s:%i:%.*s\n", filename, count, match.rm_eo - match.rm_so, line + match.rm_so);
          else
            printf("%.*s\n", match.rm_eo - match.rm_so, line + match.rm_so);
          char *remaining = line + match.rm_eo;
          while (!regexec(preg, remaining, 1, &match, 0)){
            if (flags.numberLine)
              printf("%s:%i:%.*s\n", filename, count, match.rm_eo - match.rm_so, line + match.rm_so);
            else
              printf("%.*s\n", match.rm_eo - match.rm_so, line + match.rm_so);
            remaining = remaining + match.rm_eo;
          }
        }
          else{
             if (flags.numberLine)
                printf("%s:%i:%s", filename, count, line);
              else
                printf("%s", line);
          }
      }
    }
  }
  free(line);
}

void Grep(int argc, char *argv[], Flags flags) {
  char **pattern = &argv[1];
  char **end = &argv[argc];
  int count = 0;
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
      ++count;
      if (count >= 2) break;
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
      GrepCount(file, *filename, flags, preg, count);
    } else
      GrepFile(file, flags, preg, *filename);
    fclose(file);
  }
}

int main(int argc, char *argv[]) {
  Flags flags = GrepReadFlags(argc, argv);
  Grep(argc, argv, flags);
}
