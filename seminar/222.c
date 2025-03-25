#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    }
    return p_str;
}

void swapp(char str[], int len)
{
    if (str == NULL)
    {
        return;
    }
    //printf("%s\n", str);
    char *a1;
    char *a2;
    char *buff = calloc(len, 1);
    int i = 0;
    int counter = 0;
    while (str[i] != '\0')
    {
        if (str[i] == ' ')
        {
            counter++;
        }
        if (str[i] != ' ')
        {
            if(counter > 1)
            {
                strcpy(buff,&str[i]);
                strcpy(&str[i - counter + 1],buff);
            }
            counter = 0;
        }
        i++;
    }
    //printf("%s\n", str);
    len = strlen(str);
    i = 0;
    counter = 0;
    while (str[i] != '\0')
    {
        if (str[i] == ' ')
        {
            counter++;
            if (counter == 1)
            {
                a1 = &str[i];
            }
            a2 = &str[i];
        }
        i++;
    }
    if (counter == 0)
    {
        free(buff);
        return;
    }
    char *buff2 = calloc(len,1);
    len = strlen(str);
    memcpy(buff, str, a1 - str + 1);
    buff[a1 - str] = '\0';
    //printf("\t buff: %s \n", buff);
    strcpy(buff2, a1); // ostatok v buf2
    //printf("buff2: \t%s \n", buff2);
    memcpy(str, a2+1, &str[len - 1] - a2); // w1 <- w2
    strcpy(&str[(&str[len] - a2 - 1)], buff2); // w2 + ost
    //printf("str: \t%s \n", str);
    strcpy(a2 + ((&str[len - 1] - a2) - (a1 - str - 1)), buff); // w2n <- bu
    free(buff);
    free(buff2);
    return;
}

int main(void)
{
    int len;
    char* str;
    int i = 0;
    str = ReadLn();
    len = strlen(str);
    swapp(str, len);
    if (len != 0) 
    {
    i = 0;
    while (str[i] != '\0')
    {
        fputc(str[i], stdout);
        i++;
    }
    printf("\n");
    }
    return 0;
}

