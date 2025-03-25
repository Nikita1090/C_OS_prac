#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
// ctrl + c SIGINT
static int pids = -10;
static int pidp = -10;


void sigH_F1(int s)
{
    printf("Father\n");
    kill(pids, SIGUSR1);
}

void sigH_S1(int s)
{
    printf("Son\n");
    kill(pidp, SIGUSR1);
}

void alrF(int s)
{
    kill(pids, SIGKILL);
}


int main(void)
{
    
    if((pids = fork()) > 0) // F
    {
        signal(SIGUSR1, &sigH_F1);
        signal(SIGALRM, &alrF);
        sleep(1);
        alarm(2);
        kill(getpid(), SIGUSR1);
        wait(NULL);
        for(;;)
        {
            printf("Father\n");
            sleep(1);
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

        }
    }

    return 0;
}