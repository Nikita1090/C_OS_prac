#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define n 4

struct list2
{
    char* c;
    struct list2* next;
    struct list2* prev;
    
};
                        // ИСПОЛЬЗУЕТСЯ ТОЛЬКО ДЛЯ СЛОВ. Условие вроде как разрешает это.
char* ReadW(char* fl) // модификация ввода из предыдущих дз
{                   // fl это крч флаг конец слов или нет
    int c;
    char *p_str = NULL;
    int i = 0;
    while (((c = fgetc(stdin)) != '\n') && (c != EOF) && (c != ' ')) //  по сути тут добавили доп условие на пробел
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
        if (c != ' ') // меняем флаг
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

void delete_List2(struct list2* L)
{
    if(L->next != NULL)
    {
        delete_List2(L->next);
    }
    free(L->c);
    free(L);
    return;
}

void sort_alf(struct list2* L)
{
    struct list2* L_p = L;
    int fl = 0; // флаг сортировки
    char* buf = NULL;
    if(L == NULL)
    {
        return;
    }
    while(fl == 0)
    {
        L_p = L;
        fl = 1;
        while(L_p->next != NULL)
        {
            //printf("88 %s\n", L_p->c);
            if(strcmp(L_p->c, L_p->next->c) > 0) // , max(strlen(L_p->c), strlen(L_p->next->c))
            {
                buf = L_p->c; // строки меняем местами
                L_p->c = L_p->next->c;
                L_p->next->c = buf;
                fl = 0;
            }
            L_p = L_p->next;
            
        }
        //printf("88---%s\n", L_p->c);
    }
    return;
}

int main(void)
{
    struct list2* L1 = NULL; // начало списка
    struct list2* L_last = NULL;
    char fl = 0; //  флаг конца слова 1 - \n && EOF, 2 - ' '
    char* buf = NULL;
    while (fl != 1)
    {
        buf = ReadW(&fl); // Считали слово, сохранили ссылку на него
        if(buf == NULL)
        {
            break;
        }
        if(L1 == NULL)
        {
            L1 = calloc(1, sizeof(struct list2));
            L1->c = buf;
            L1->next = NULL;
            L1->prev = NULL;
            L_last = L1;
        }
        else
        {
            L_last->next = calloc(1, sizeof(struct list2));
            L_last->next->prev = L_last;
            L_last = L_last->next;
            L_last->c = buf;
            L_last->next = NULL;
        }
    }
    if(L1 != NULL)
    {
        struct list2* L2 = L1; // для прохода по списку
        printf("\n-------------\n");
        printf("Список:\n");
        while(L2->next != NULL) // -->
        {
            printf("%s ", L2->c);
            L2 = L2->next;
        }
        printf("%s ", L2->c); // чтобы L2 ссылался на последний элем списка сделал так
        printf("\n-------------\n");
        printf("Список в обр. порядке:\n");
        while(L2 != NULL) // <--
        {
            printf("%s ", L2->c);
            L2 = L2->prev;
        }
        printf("\n-------------\nВ алф. порядке: \n");
        sort_alf(L1);
        L2 = L1;
        while(L2 != NULL) // -->
        {
            printf("%s ", L2->c);
            L2 = L2->next;
        }
        delete_List2(L1); // чистим мусор за собой
        printf("\n");
    }
    return 0;
}