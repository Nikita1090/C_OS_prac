#include <stdio.h>

void SortF(int a[], int len)
{
    
    for (int c = 0; c<len; c++)
    {
        int buf;
        for(int i = len - 1; i>0; i--)
        {
            if (a[i] < a[i - 1])
            {
                buf = a[i];
                a[i] = a[i - 1];
                a[i - 1] = buf;
            }
        }
    }
    return;
}

int main(void)
{
    int arr[] = {4, 3, 8, 0};
    for (int i = 0; i<4; i++)
    {
        printf("%d\t", arr[i]);
    }
    printf("\n");
    SortF(arr, 4);
    for (int i = 0; i<4; i++)
    {
        printf("%d\t", arr[i]);
    }
    printf("\n");
    return 0;
}
 

