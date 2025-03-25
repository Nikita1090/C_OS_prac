#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>	
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char ** argv)
{
    int fd1 = open(argv[1], O_WRONLY | O_CREAT, 0600);
    lseek(fd1, 0, SEEK_SET);
    if (fd1 < 0)
    {
        fprintf(stderr, "Ошибка создания\n");
        return -1;
    }
    if (argc < 2)
    {
        return -1;
    }
    int ret;
    for(int i = 23; i<29; i++)
    {
        ret = write(fd1, &i, sizeof(int));
        if(ret < 0)
        {
            fprintf(stderr, "Ошибка записи\n");
            return -1;
        }
    }
    for(int i = -4; i<3; i++)
    {
        ret = write(fd1, &i, sizeof(int));
        if(ret < 0)
        {
            fprintf(stderr, "Ошибка записи\n");
            return -1;
        }
    }
    if(close(fd1) != 0)
    {
        return -1;
    }
    return 0;
}