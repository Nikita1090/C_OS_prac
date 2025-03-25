#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAXLEN 100
// ctrl + c SIGINT
static int pids;
static int pidp;
int file;
char str1[MAXLEN];
char str2[MAXLEN];

struct timespec tw = {0, 100};

int readW(int file, char* str1)
{
    char c;
    int i = 0;
    ssize_t err;
    while((err = read(file, &c, 1)) && (c != ' ') && (i<MAXLEN) && (c != '\n') && (err > 0))
    {
        str1[i] = c;
        i++;
    }
    str1[i] = '\0';
    return (int) err;
}

void sigH_F1(int s)
{
    nanosleep(&tw, NULL);
    if(readW(file, str1) == 0)
        exit(0); 
    printf("Father: %s\n", str1);
    kill(pids, SIGUSR1);
}

void sigH_S1(int s)
{
    nanosleep(&tw, NULL);
    if(readW(file, str2) == 0)
        exit(0); 
    printf("Son: %s\n", str2);
    kill(pidp, SIGUSR1);
}

void alrF(int s)
{
    kill(pids, SIGINT);
    exit(0);
}


int main(int argc, char** argv)
{
    if(argc != 2)
        return -1;
    file = open(argv[1], O_RDONLY);
    if( file == -1)
        abort();
    if((pids = fork()) > 0) // F
    {
        signal(SIGUSR1, &sigH_F1);
        signal(SIGALRM, &alrF);
        sleep(1);
        alarm(1);
        kill(getpid(), SIGUSR1);
        for(;;)
        {

        }
    }
    else if(pids != 0)
    {
        abort();
    }
    else // S
    {
        pidp = getppid();
        signal(SIGUSR1, &sigH_S1);
        for(;;)
        {
            sleep(10);
        }
    }

    return 0;
}