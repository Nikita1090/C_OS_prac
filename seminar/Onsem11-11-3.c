#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>	
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

char* intRfl_to_str(int n)
{
    char* temp = "rwxrwxrwx";
    int len = strlen(temp);
    //if((n >= 1024) || (n < 0))
    //{
       // return NULL;
    //}
    char* str = calloc(10, sizeof(char));
    str[9] = '\0';
    int mask = 1 << 8;
    for(int i = 0; i<len; i++)
    {
        if(mask & n)
        {
            str[i] = temp[i];
        }
        else
        {
            str[i] = '-';
        }
        mask >>= 1;
    }
    return str;
}


int main(int argc, char *argv[])
{
    DIR* d = opendir("./");
    struct dirent *s;
    struct stat stat1;
    char* str;
    while((s = readdir(d)) != NULL)
    {
        printf("%20s ", s->d_name);
        lstat(s->d_name, &stat1);
        printf("%10d ", (int) stat1.st_ino);
        if(S_ISREG(stat1.st_mode))
        {
            printf("REGULAR ");
        }
        else if(S_ISDIR(stat1.st_mode))
        {
            printf("DIR     ");
        }
        else if(S_ISCHR(stat1.st_mode))
        {
            printf("SYM_div ");
        }
        else if(S_ISBLK(stat1.st_mode))
        {
            printf("B_div   ");
        }
        else if(S_ISFIFO(stat1.st_mode))
        {
            printf("FIFO_CH ");
        }
        else if(S_ISLNK(stat1.st_mode))
        {
            printf("SYM_PNT ");
        }
        else if(S_ISSOCK(stat1.st_mode))
        {
            printf("SOCKET  ");
        }
        else
        {
            printf("UNKNOWN ");
        }
        str = intRfl_to_str(stat1.st_mode);
        printf("%10s", str);
        free(str);
        printf("\n");
        
    }
    closedir(d);
    return 0;
}