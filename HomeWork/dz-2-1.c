#include <stdio.h>
#include <limits.h>

#define size_m 256

void spyralika(int d1, int d2, int arr[][size_m])
{
    int bound = 0;
    int k = 1;
    int i = 0;
    int j = 0;
    while (k <= d1 * d2)
    {
        arr[i][j] = k;
        if (i == bound && j < d2-bound-1)
        {
            j++;
        }
        else if (j == d2-bound-1 && i < d1-bound-1)
        {
            i++;
        }
        else if (i == d1-bound-1 && j > bound)
        {
            j--;
        }
        else
        {
            i--;
        }
        if ((i == bound + 1) && (j == bound) && (bound != d2 - bound - 1))
        {
            bound++;
        }
        k++;
    }
}

int main()
{
    int M;
    int N;
    scanf("%d%d", &M, &N);
    int arr[size_m][size_m] = {0};
    spyralika(M, N, arr);
    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            printf("%d\t", arr[i][j]);
        }
        printf("\n");
    } 
}