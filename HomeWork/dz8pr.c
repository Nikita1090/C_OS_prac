#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>	
#include <string.h>

int main(int argc, char *argv[])
{
    if(argc != 2)
        return -1;
    int n = atoi(argv[1]);
    for(int i = 0; i<n; i++)
    {
        printf("My process id is %9d\n", (int) getpid());
    }
    exit(0); // exit(1); abort();
}