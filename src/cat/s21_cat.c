#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    bool number_empty;
    bool number_all;
    bool show_endl;
    bool squeeze;
    bool show_tabs;
    bool error;
} CatInfo;

bool PrintNoArgs(int fd, const char *name) {
    char buf[1024];
    ssize_t bytes_read;

    while ((bytes_read = read(fd, buf, sizeof(buf))) > 0) {
        if (fwrite(buf, 1, (size_t)bytes_read, stdout) != (size_t)bytes_read) {
            fprintf(stderr, "%s: error writing to stdout\n", name);
            return false;
        }
    }

    if (bytes_read == -1) {
        fprintf(stderr, "%s: error reading from file\n", name);
        return false;
    }

    return true;
}

bool ParseArg(CatInfo *info, char *arg, char *name) {
    if (*arg == '-') {
        if (strcmp(arg, "-b") == 0) {
            info->number_empty = true;
        } else if (strcmp(arg, "-e") == 0) {
            info->show_endl = true;
        } else if (strcmp(arg, "-n") == 0) {
            info->number_all = true;
        } else if (strcmp(arg, "-s") == 0) {
            info->squeeze = true;
        } else if (strcmp(arg, "-t") == 0) {
            info->show_tabs = true;
        } else {
            fprintf(stderr, "%s: invalid option -- '%s'\n", name, arg);
            info->error = true;
            return false;
        }
    } else {
        info->error = true;
        fprintf(stderr, "%s: invalid option -- '%s'\n", name, arg);
        return false;
    }

    return true;
}

bool ProcessArgs(int argc, char *argv[]) {
    CatInfo info = {0};

    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            if (!ParseArg(&info, argv[i], argv[0])) {
                return false;
            }
        } else {
            int fd = open(argv[i], O_RDONLY);
            if (fd == -1) {
                fprintf(stderr, "%s: cannot open '%s': No such file or directory\n", argv[0], argv[i]);
                info.error = true;
            } else {
                if (!PrintNoArgs(fd, argv[0])) {
                    info.error = true;
                }
                close(fd);
            }
        }
    }

    return !info.error;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        return PrintNoArgs(STDIN_FILENO, argv[0]) ? 0 : 1;
    } else {
        return ProcessArgs(argc, argv) ? 0 : 1;
    }
}
