#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void CatNoArgs(int fd){
    char buf[4096];
    int bytes_read;

    bytes_read = read(fd, buf, 4096);
    while (bytes_read != -1){
       printf("%.*s", bytes_read, buf);
       bytes_read = read(fd, buf, 4096);
    }
    
}

void CatArgs(int argc, char *argv[]){
    (void)argc;
    CatNoArgs(open(argv[1], O_RDONLY));
}

int main(int argc, char *argv[]){
    (void)argv;
    if (argc == 1)
        CatNoArgs(STDIN_FILENO);
    else
        CatArgs(argc, argv);
    return 0;
}