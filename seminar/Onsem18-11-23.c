#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define MAX_STR 100

int main(int argc, char ** argv)
{
    int pid;
    int fd1[2];
    int fd2[2];
    if(argc != 2)
        return -1;
    int file = open(argv[1], O_WRONLY | O_CREAT);
    if (file < 1)
        return 1;
    if(pipe(fd1) == -1)
        return 2;
    if(pipe(fd2) == -1)
        return 2;
    if (pid = fork())
    {
        char* str = calloc(MAX_STR+1, sizeof(char));
        char c;
        char cn; // це костыль для того чтобы считать с во второй раз
        int len;
        while(1)
        {
            scanf("%c", &c);
            scanf("%s", str);
            scanf("%c", &cn); // "\n"
            len = strlen(str);
            if(strcmp(str, "exit") == 0)
            {
                len = -1;
                write(fd1[1], &c, sizeof(char));
                write(fd1[1], &len, sizeof(int));
                break;
            }
            write(file, str, len * sizeof(char));
            write(fd1[1], &c, sizeof(char));
            write(fd1[1], &len, sizeof(int));
            write(fd1[1], str, sizeof(char) * len);
            read(fd2[0], &len, sizeof(int));
            
        }
        printf("Parent is going to be terminated\n");
        close(fd1[1]);
        close(fd1[0]);
        close(fd2[1]);
        close(fd2[0]);
        close(file);
        free(str);
        return 0;
    }
    else if (pid == -1)
    {
        return -3;
    }
    else
    {
        char strb[MAX_STR+1];
        char cc;
        int llen;
        int counter;
        while(1)
        {
            counter = 0;
            read(fd1[0], &cc, sizeof(char));
            read(fd1[0], &llen, sizeof(int));
            if(llen == -1)
            {
                break;
            }
            read(fd1[0], strb, sizeof(char)*llen);
            for(int i = 0; i<llen; i++)
            {
                if(strb[i] == cc)
                {
                    counter++;
                }
            }
            sprintf(strb, " %d\n", counter);
            llen = strlen(strb);
            write(file, strb, llen * sizeof(char));
            llen = 0;
            write(fd2[1], &llen, sizeof(int));
        }
        printf("Child is going to be terminated\n");
        close(fd1[1]);
        close(fd1[0]);
        close(fd2[1]);
        close(fd2[0]);
        close(file);
        return 0;
    }
}