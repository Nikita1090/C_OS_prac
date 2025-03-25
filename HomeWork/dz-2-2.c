#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int how_many_1s(int a)
{
    int mask = 1;
    int i = 0;
    while (a != 0)
    {
        if ((a & mask) != 0)
        {
            i++;
        }
        a >>= 1;
    }
    return i;
}


void binary(int a)
{
    int mask = INT_MIN;
    for (int i = 0; i < 32; i++)
    {
        if ((a & mask) != 0)
        {
            printf("1");
        }
        else
        {
            printf("0");
        }
        a <<= 1;
    }
    printf("\n");
}

void sort101(int arr[], int d1)
{
    int buf;

    int a, b;
    for (int j = 0; j < d1; j++)
    {
        for (int i = 1; i < d1; i++)
        {
            a = how_many_1s(arr[i]);
            b = how_many_1s(arr[i-1]);
            if(a < b)
            {
                buf = arr[i];
                arr[i] = arr[i - 1];
                arr[i - 1] = buf;
            }
            if(a == b)
            {
                if(arr[i] > arr[i - 1])
                {
                buf = arr[i];
                arr[i] = arr[i - 1];
                arr[i - 1] = buf;
                }
            }
        }
        
    }
    return;
}

void arr_out(int a[], int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
    ///////////
    for (int i = 0; i < len; i++)
    {
        binary(a[i]);
    }
    printf("\n");
    return;
}



int main(void)
{
    int mass[5];// = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; i++)
    {
        scanf("%d", &mass[i]);
    }
    
    sort101(mass, 5);

    arr_out(mass, 5);
    return 0;
}