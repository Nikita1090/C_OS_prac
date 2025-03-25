
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#define n 4

char* ReadF(char* fl, FILE* F) // модификация ввода из предыдущих дз
{                   // fl это крч флаг конец файла или нет
    int c;
    char *p_str = NULL;
    int i = 0;
    while (((c = fgetc(F)) != '\n') && (c != EOF))
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
    if (fl != NULL)
    {
        if (c == EOF) // меняем флаг
        {
            *fl = 1;
        }
        else
        {
            *fl = 0;
        }
    }
    return p_str;
}

int main(int argc, char** argv)
{

    static struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt); // сохранение текущего режима

    newt = oldt;

    newt.c_lflag &= ~(ICANON);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // изменение

    char many_to_one_str = 0; // скип пустых строк в одну
    int str_in_one = 1; // сколько строк за раз выводить
    int shift_file = 0; // сдвиг в файле
    if (2 != argc)
    {
        for(int i = 1; i<argc - 1; i++)
        {
            if (strcmp(argv[i], "-s") == 0)
            {
                many_to_one_str = 1; 
                printf("a1\n");  
            }
            else if ((argv[i])[0] == '-')
            {
                str_in_one = atoi((argv[i])+1);
                printf("a2=%d\n", str_in_one); 
            }
            else
            {
                shift_file = atoi((argv[i])+1);
                printf("a3--%d\n", shift_file); 
            }
        }
    }                // получили все аргументы их командной строки
    FILE * F = fopen(argv[argc-1], "r");
    if (F == NULL)
    {
        return -2;
    }
    //printf("argfine\n");
    char c; 
    char fl = 0;
    char* str = NULL; // все нужные переменные
    char skipped = 0;
    char skip = 0;
    int time_s_skip = str_in_one; // счётчик пропускаемых строк
    while ((skip) || ((scanf("%c", &c) != EOF) && c != 'q'))
    {   
        //printf("!!!!!!%c\n", c);
        str = ReadF(&fl, F);
        //printf("\tSTRRRRR%s\n", str);
        if(fl == 1)
        {
            free(str);
            break;
        }
        if(shift_file > 0) // смещение по файлу
        {
            shift_file--;
            skip = 1;
            free(str);
            continue;
        }
        if ((str[0] == '\0') && (many_to_one_str == 1)) //  скип пробелов через перем skip
        {
            if (skipped)
            {
                free(str);
                skip = 1;
                continue;
            }
            skipped = 1;
        }
        else
        {
            skipped = 0;
        }
        skip = 0;
        if (str_in_one > 1) // вывод нескольких за раз
        {
            if(time_s_skip > 1)
            {
                time_s_skip--;
                skip = 1;
            }
            else
            {
                skip = 0;
                time_s_skip = str_in_one;
            }

        }
        printf("%s\n", str);
        free(str); // каждую итерацию цикла новая ссылка на новую строку, старые нужно удалять
    }



    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // восстановление исходного режима
    fclose(F);
    return 0;
}