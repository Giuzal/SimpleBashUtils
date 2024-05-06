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

bool CatNoArgs(int fd, char *name){
    char buf[4096];
    int bytes_read;

    if (fd == -1){
        perror(name);
        return 1;
    }

    bytes_read = read(fd, buf, 4096);
    while (bytes_read != -1){
       printf("%.*s", bytes_read, buf);
       bytes_read = read(fd, buf, 4096);
    }
    return bytes_read != -1;
}

bool CatParseArg(CatInfo *info, char *argv, char *name){
    ++argv;
    if (*argv=='-'){
        ++argv;
        if (strcmp(argv,"number-nonblank") == 0){
            info->number_empty = true;
        } else if (strcmp(argv, "number") == 0){
            info->number_all = true;
        } else if (strcmp(argv, "squeeze-blank") == 0){
            info->squeeze = true;
        } else{
            info->error = false;
            fprintf(stderr, "%s: invalid option-- '%s'\n", name, argv);
            return false;
        }
        return true;
    }
    for (char *it = argv; *it != '\0'; ++it){
        switch (*it) {
            case 'b':
            info->number_empty = true;
            break; case 'e':
            info->show_endl = true;
            break; case 'n':
            info->number_all = true;
            break; case 's':
            info->squeeze = true;
            break; case 't':
            info->show_tabs = true;
            break; default: 
            fprintf(stderr, "%s: invalid option- '%s'\n", name, argv);
            info->error = false;
            return 1;
        }
    }
    return true;
}

void CatArgsHelper(CatInfo info, char *ptr, char *end, bool *empty){
        if (end == ptr){
            if(!info.squeeze){
                printf("\n");
                return;
            }
            if(!*empty){
                printf("\n");
            *empty = true;
            return;
        } 
        else 
            *empty = false;
        if (!end)
            printf("%s", ptr);
        else 
            printf("%.*s", (int)(end - ptr + 1), ptr);
}
}

bool CatArgsPerform(CatInfo *info, char *file, char *name){
    char buf[4096];
    int fd;
    int bytes_read;
    char *end;
    char *ptr;
    bool empty = false;

    fd = open(file, O_RDONLY);
    if (fd == -1){
        perror(name);
        return false;
    }

    bytes_read = read(fd, buf, 4095);

    while (bytes_read > 0){
        buf[bytes_read] = 0;
        ptr = buf;
        while(1){
            end = strchr(ptr, '\n');
            CatArgsHelper(*info, ptr, end, &empty);
            if(end == 0) break;
            ptr = end + 1;
        }
        bytes_read = read(fd, buf, 4095);
    }
    return bytes_read != -1;
}

bool CatArgs(int argc, char *argv[]){
    CatInfo info = {0, 0, 0, 0, 0, true};
    for (int i = 1; i != argc; ++i){
        if(*argv[i] == '-'){
           if(!CatParseArg(&info, argv[i], argv[0])){
            return false;
           }
        }
    }
    if (!(info.number_empty || info.number_all ||
         info.show_endl || info.squeeze || info.show_tabs)){
            for (int i = 1; i != argc; ++i){
                if(*argv[i] != '-'){
                    if (!CatNoArgs(open(argv[i], O_RDONLY), argv[0]))
                        info.error = false;
                    }
                 }
            }
    else {
            for (int i = 1; i != argc; ++i){
                if(*argv[i] != '-'){
                    if (!CatArgsPerform(&info, argv[i], argv[0]))
                        info.error = false;
                    }
                 }
            }
     return info.error;
}

int main(int argc, char *argv[]){
    (void)argv;
    if (argc == 1)
        CatNoArgs(STDIN_FILENO, argv[0]);
    else{
        if (CatArgs(argc, argv)){
            return 1;
        }
    }
    return 0;
}