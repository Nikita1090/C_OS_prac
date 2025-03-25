#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>	
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

char* intRfl_to_str(int n)
{
    char* temp = "rwxrwxrwx";
    int len = strlen(temp);
    // 1111
    // 1000
    if((n >= 512) || (n < 0))
    {
        return NULL;
    }
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
    int fl = 1;
    char* str = intRfl_to_str(fl);
    if (str != NULL)
    {
        printf("%s\n", str);
        free(str);
    }
    fl = -1;
    str = intRfl_to_str(fl);
    if (str != NULL)
    {
        printf("%s\n", str);
        free(str);
    }
    else
    {
        printf("NULL\n");
    }
    fl = 511;
    str = intRfl_to_str(fl);
    if (str != NULL)
    {
        printf("%s\n", str);
        free(str);
    }
    return 0;
}