#include <stdio.h>

int main(void)
{
    int arr[] = {1, 2, 3, 4};
    for (int i = 0; i<4; i++)
    {
        printf("%d\t", arr[i]);
    }
    printf("\n");
    UshiftF(arr, 4, 3);
    for (int i = 0; i<4; i++)
    {
        printf("%d\t", arr[i]);
    }
    printf("\n");
    return 0;
}
 

void UshiftF(int a[], int len, int n)
{
    n = n % len;
    for (int c = 0; c<n; c++)
    {
        int buf = a[len - 1];
        for(int i = len - 1; i>0; i--)
        {
            a[i] = a[i - 1];
        }
        a[0] = buf;
    }
    return;
}