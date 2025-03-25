#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

#define n 4

struct List
{
    char* str;
    int count;
    struct List* next;
};



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
void deleteL(struct List* L) // удаление (в этот раз без рекурсии)
{
    struct List* Lp = L;
    while(Lp != NULL)
    {
        L = Lp;
        Lp = Lp->next;
        free(L->str);
        free(L);
    }
    return;
}

struct List* append(struct List* L, char* new, int shift) // добавление элемента или коррекция поля количества повторений у существующего
{
    int newlenOk = strlen(new) > shift;
    if(L == NULL) // если список пустой
    {
        L = calloc(1, sizeof(struct List));
        L->next = NULL;
        L->count = 1;
        L->str = new;
        return L;
    }
    struct List* L1 = L;
    struct List* L2 = L; // костыль для корректной работы
    while(L1 != NULL)
    {
        L2 = L1;
        if(newlenOk && (strlen(L1->str) > shift)) // имеет ли смысл сдвиг?
        {
            if((strcmp(&L1->str[shift], &new[shift]) == 0)) 
            {
                free(new); // если элем уже есть то строка не нужна
                L1->count++;
                return L;
            }
        }
        L1 = L1->next;
    }
    L1 = L2;
    L1->next = calloc(1, sizeof(struct List)); // новый элем в конец списка
    L1 = L1->next;
    L1->next = NULL;
    L1->count = 1;
    L1->str = new;
    return L;
}

int uniq(int argc, ...)
{
    char* argv[argc];
    va_list factor;
    va_start(factor, argc);
    for (int i = 1; i<argc; i++)
    {
        argv[i] = va_arg(factor, char*);
    }
    va_end(factor);
    char count_fl = 0; // надо выводить кратность?
    char no_unic = 0; // уникальные строки не выводить
    char unic_only = 0; // только уникальные
    int shift_str = 0; // смещение в строках при сравнении
    if (2 != argc)
    {
        for(int i = 2; i<argc; i++)
        {
            if (strcmp(argv[i], "-s") == 0)
            {
                shift_str = atoi(argv[i+1]);
                i++;  
            }
            else if ((argv[i])[0] == '-')
            {
                if((argv[i])[1] == 'c')
                {
                    count_fl = 1;
                }
                else if ((argv[i])[1] == 'd')
                {
                    no_unic = 1;
                }
                else if ((argv[i])[1] == 'u')
                {
                    unic_only = 1;
                }
            }
        }
    }                // получили все аргументы их командной строки
    if ((no_unic + unic_only) == 2) // два противоречущих друг другу флага, смысла в выводе и чтении нет
    {
        printf("Нет подходящих строк\n");
        return 0;
    }
    FILE * F = fopen(argv[1], "r");
    if (F == NULL)
    {
        return -2;
    }
    char fl = 0;
    char* str = NULL; // все нужные переменные
    struct List* LL = NULL;
    while (1)
    {   
        str = ReadF(&fl, F); // fl маркер конца файла
        if(fl == 1)
        {
            LL = append(LL, str, shift_str);
            break;
        }
        LL = append(LL, str, shift_str); // append за нас разберётся
    }
    struct List* L1 = LL;
    while(L1 != NULL)
    {
        if (((no_unic || unic_only) == 0) || (no_unic && (L1->count != 1)) || (unic_only && (L1->count == 1))) // все 3 условия когда нам нужно выводить строку
        {
            if (count_fl)
            {
                printf("=%d=", L1->count);
            }
            printf("%s\n", L1->str);
        }
        L1 = L1->next;
    }

    deleteL(LL); // ничего не забываем
    fclose(F);
    return 0;
}

int main(void)
{
    uniq(5, "testf", "-u", "-s", "1");
    return 0;
}