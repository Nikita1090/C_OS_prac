#include <stdio.h>
#include <stdlib.h>
// алгоритм известен с прошлых задач
// вариант без использования переменных)
#define BINPRINT(a) \
{\
    if (((((typeof(a))1) << (8*sizeof(typeof(a)) - 1)) & a) == 0)\
    {\
        printf("0");\
    }\
    else\
    {\
        printf("1");\
    }\
    for(int i = 1; i < 8*sizeof(typeof(a)); i++)\
    {\
        if (((((typeof(a))1) << (sizeof(typeof(a))*8 - (i+1))) & a) == 0)\
        {\
            printf("0"); \
        }\
        else\
        {\
            printf("1"); \
        }\
    }\
    printf("\n"); \
}


int main(void) 
{
    // код из условия (в этот раз работает)
    char c = -3;
    if(c)
        BINPRINT(c)
    else
        printf("Zero!\n");
    BINPRINT((short)7);
    long x = 15;
    if(x)
        BINPRINT(x)
    else
        printf("Zero!\n");
    BINPRINT((long long)x);
    return 0;
}