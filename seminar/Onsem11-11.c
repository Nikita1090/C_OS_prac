#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>	
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


unsigned int Txtflags_to_int(const char* str)
{
    char* temp = "rwxrwxrwx";
    //            r-x-w----
                //101010000
    int len = strlen(temp);
    int res = 0;
    if(str == NULL)
    {
        return -1;
    }
    for(int i = 0; i<len; i++)
    {
        if(str[i] == '\0')
        {
            return -1;
        }
        else if(str[i] == '-')
        {
            res <<= 1;
        }
        else if(str[i] == temp[i])
        {
            res <<= 1;
            res++;
        }
        else
        {
            return -1;
        }
    }
    return res;
}


int main(int argc, char *argv[])
{
    unsigned fl = Txtflags_to_int("132");
    printf("%o\n", fl);
    fl = Txtflags_to_int("--------x");
    printf("%o\n", fl);
    fl = Txtflags_to_int("rwxrwxrwx");
    printf("%o\n", fl);
    return 0;
}