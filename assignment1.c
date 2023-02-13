#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
int main(int argc, char** argv)
{
    int fd1 = open(argv[1], O_RDONLY);
    int fd2;
    if(fd1 < 0)
    {
        perror("No source file.");
        exit(1);
    }
    if(argc == 3)
    {
        char ch;
        printf("Override target file y/n?");
        scanf("%c", &ch);
        if(ch == 'y')
        {
            fd2 = open(argv[2], O_WRONLY);
        }
        else if(ch == 'n')
        {
            fd2 = open(argv[2], O_WRONLY | O_APPEND);
        }
    }
    char buffer[256];
    ssize_t bytes;
    while ((bytes = read(fd1, buffer, 256)) != 0)
    {
        write(fd2, buffer, bytes);
    }
    close(fd1);
    close(fd2);
    return 0;
}
