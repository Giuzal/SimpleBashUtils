#include <stdio.h>
#include <unistd.h>

void CatNoArgs(){
    char buf[4096];
    int fd = STDIN_FILENO;
    int bytes_read;

    bytes_read = read(fd, buf, 4096);
    while (bytes_read != -1){
       printf("%.*s", bytes_read, buf);
       bytes_read = read(fd, buf, 4096);
    }
    
}

int main(int argc, char *argv[]){
    (void)argv;
    if (argc == 1)
        CatNoArgs();
    return 0;
}