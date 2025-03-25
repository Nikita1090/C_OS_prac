#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *str_find(char *src, char *examp)
{
    char *ptr = NULL;
    int len_src = strlen(src);
    int len_examp = strlen(examp);
    if (len_src == 0 || len_examp == 0)
    {
        return ptr;
    }
    int i = 0;
    while((i < len_src) && (len_src >= len_examp + i))
    {
        if(src[i] == examp[0])
        {
            ptr = &src[i];
            int j = 1;
            while((j<len_examp) && (src[i+j] == examp[j]))
            {
                j++;
            }
            if(j == len_examp)
            {
                break;
            }
        }
        i++;
    }
    return ptr;
}

#define n 4

char * ReadLn()
{
    int c;
    char *p_str = NULL;
    //p_str = malloc(sizeof(int) * n);
    int i = 0;
    while (((c = fgetc(stdin)) != '\n') && (c != EOF))
    {
        if (i % n == 0)
        {
            p_str = realloc(p_str, n + i);
        }
        if (p_str != NULL)
        {
            p_str[i] = (char) (c % 256);
        }
        else
        {
            printf("ERROR MEM");
            break;
        }
        i++;
    }
    if (i % n == 0 && p_str != NULL)
    {
        p_str = realloc(p_str, n + i);
        if (p_str != NULL)
        {
            p_str[i] = '\0';
        }
        else
        {
            printf("ERROR MEM");
            return NULL;
        }
    }
    else
    {
        if (p_str != NULL)
        {
            p_str[i] = '\0';
        }
        else
        {
            p_str = malloc(1);
            *p_str = '\0';
        }
    }
    return p_str;
}






int main(void)
{
    char* str1 = ReadLn();
    char* str2 = ReadLn();
    char* res = str_find(str1, str2);
    if (res == NULL)
    {
        printf("Нет вхождений\n");
    }
    else
    {
        printf("%s\n", res);
    }
    free(str1);
    free(str2);
    return 0;
}