#include <stdio.h>
#include <limits.h>

void binary(int n)
{
    int mask = INT_MIN;
    int buf;
    for (int i = 0; i < 32; i++)
    {
        if (n & mask)
        {
            printf("1");
        }
        else
        {
            printf("0");
        }
        n <<= 1;
    }
    printf("\n");
}

int cycle_shift(unsigned int n, int dig, int dr_fl)
{
    dig = dig % 32; 
    int check;
    int OneBack;
    if (dr_fl == 1)
    {
        int mask2 = 1;
        for (int i = 0; i < dig; i++)
        {
            OneBack = (n & mask2); 
            n >>= 1;
            if (OneBack)
                n = n | INT_MIN; 
        }
    }
    else
    {
        int mask2 = INT_MIN;
        for (int i = 0; i < dig; i++)
        {
            OneBack = ((n & mask2) != 0); 
            n <<= 1;
            if (OneBack)
                n = n | 1; 
        }
    }
    return n;
}

int main()
{
    int n;
    scanf("%d", &n);
    binary(n);
    int digits;
    printf("\nHow big shift: ");
    scanf("%d", &digits);
    int dr_fl_flag;
    printf("Shift to? (1-right, 2-left): ");
    scanf("%d", &dr_fl_flag);
    n = cycle_shift(n, digits, dr_fl_flag);
    binary(n);
    return 0;
} 