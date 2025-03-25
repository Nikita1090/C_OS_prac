#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <string.h>
#include <time.h>

char * reverse_Str(char* str, int len)
{
    char* buff = malloc(len + 1);
    int i = len - 1;
    int shift = 0;
    int previ = len;
    buff[len] = '\0';
    while(str[i] == ' ')
    {
        buff[shift] = ' ';
        shift++;
        previ--;
        i--;
    }
    while(i>=0)
    {
        if(str[i] == ' ')
        {   if(str[i+1] != ' ')
            {
                
                memcpy((buff+shift), &str[i+1], previ - i - 1);
                shift += previ - i - 1;
                buff[shift] = ' ';
                shift++;
                previ = i;
                
            }
            else
            {
                buff[shift] = ' ';
                shift++;
                previ = i;
            }
        }
        else if (i == 0)
        {
            //if(str[i+1] != ' ')
            //{
            memcpy((buff+shift), &str[i], previ - i);
            //}
        }
        i--;
        
        
    }
    buff[len] = '\0';
    printf("\n%s|\n", buff);
    free(buff);
    return str;
}



int main(int argc, char *argv[])
{
    int max_str_len = 1;
    double sum = 0.0;
    char* end_p = NULL;
    double x = 0.0;
    for (int i = 1; i<argc; i++)
    {
        x = strtod(argv[i], &end_p);
        max_str_len += strlen(argv[i]);
        if (*end_p == 0)
        {
            sum += x;
        }
    }
    printf("\n%.3lf\n", sum);
    char* str = malloc(max_str_len); //  склееваем строку
    int shift = 0;
    for (int i = 1; i<argc; i++)
    {
        strcpy(&str[shift], argv[i]);
        shift += strlen(argv[i]);
    }
    printf("%s\n", str);
    int stime; // установки рандома
    long Ltime;
    Ltime = time(NULL);
    stime = (unsigned int) Ltime / 2;
    srand(stime);

    sum = 0; //   кол-во символов
    shift  = 0;
    int prob = 0; // количество пробелов
    while(sum<max_str_len)
    {
        sum += (rand() % 6);
        prob++;
    }
    srand(stime);
    char* buf = malloc(max_str_len+prob);
    str = realloc(str, (max_str_len = max_str_len+prob));
    for(int i = 0; i < prob; i++)
    {
        shift += (rand() % 6);
        strcpy(buf, &str[shift]);
        str[shift] = ' ';
        strcpy(&str[shift+1], buf);
    }
    printf("%s|\n", str);
    str = reverse_Str(str, strlen(str));
    //printf("1111");
    free(str);
    free(buf);
    return 0;
}