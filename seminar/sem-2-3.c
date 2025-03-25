#include <stdio.h>
#include <limits.h>

#define n1 3
#define m1 3

#define n2 2
#define m2 2


void *overlay_lord(int a1[n1][m1], int a2[n2][m2], int y, int x) // поменял x y местами иначе наоборот работает
{
    static int arr[n1][m1];
    for (int i = 0; i < n1; i++)
    {
        for (int j = 0; j < m1; j++)
        {
            arr[i][j] = a1[i][j];
        }
    }
    for (int i = 0; i < n2; i++)
    {
        for (int j = 0; j < m2; j++)
        {
            if (((x + i) < n1) && ((y + j) < m1))
            {
                if ((arr[x + i][y + j] > 0) && (a2[i][j] > 0) && (INT_MAX - a2[i][j] < arr[x + i][y + j]))
                {
                    arr[x + i][y + j]=INT_MAX;
                } 
                else if((arr[x + i][y + j] < 0) && (a2[i][j] < 0) && (INT_MIN - a2[i][j] > arr[x + i][y + j]))
                {
                    arr[x + i][y + j]=INT_MIN;
                }
                else
                {
                    arr[x + i][y + j]+=a2[i][j];
                }
            }
        }
    }
    return (int *) arr;
}

void arr_input(int *arr, int d1, int d2)
{
    int dd;
    for (int i = 0; i < d1; i++)
    {
        for (int j = 0; j < d2; j++)
        {
            scanf("%d", &dd);
            arr[i + d2*j] = dd;
        }
    }
}
void arr_output(int const *arr, int d1, int d2)
{
    for (int i = 0; i < d1; i++)
    {
        for (int j = 0; j < d2; j++)
        {
            printf("%d ", arr[i + d2*j]);

        }
        printf("\n");
    }
}

int main(void)
{
    int a1[n1][m1];
    int a2[n2][m2];
    arr_input(a1, n1, m1);
    printf("\n-------\n");
    arr_input(a2, n2, m2);
    printf("\n-------\nx, y?\n");
    int x, y;
    scanf("%d", &x);
    scanf("%d", &y);
    int *arr = overlay_lord(a1, a2, x, y);
    arr_output(arr, n1, m1);

    return 0;
}


