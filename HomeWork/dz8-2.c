#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>	

void reason_end(int status)
{
    if(WIFEXITED(status))
    {
        printf("Процесс успешно завершился\n");
    }
    else
    {
        printf("Сигнал остановки: %d\n", WTERMSIG(status));
    }
    return;
}




int main(int argc, char *argv[])
{
    pid_t s_pid;
    pid_t s_pid2;
    int exfl;
    int err;
    if(argc < 6)
        return -1;
    if(argv[5][1] == 'p') // Параллельно
    {
            ////////////////////////////////////////////fork start
        if((s_pid = fork()) > 0) //dad \/
        {
            if((s_pid2 = fork()) > 0) //dad
            {
                pid_t pidb;
                pidb = wait(&err); // ждём 1й или 2й 
                if (pidb == s_pid)
                {
                    printf("Process 1 ended up with code: %5d ", err);
                }
                else
                {
                    printf("Process 2 ended up with code: %5d ", err);
                }
                reason_end(err);
                pidb = wait(&err); // ждём последний 
                if (pidb == s_pid)
                {
                    printf("Process 1 ended up with code: %5d ", err);
                }
                else
                {
                    printf("Process 2 ended up with code: %5d ", err);
                }
                reason_end(err);
            }
            else if(s_pid2 == 0) // son2
            {
                exfl = execl(argv[3], argv[3], argv[4], NULL);
                if(exfl == -1)
                {
                    printf("process fail2\n");
                    abort();
                }
            }
            else
            {
                exit(-1); // неудача 2 fork()
            }
        }
        else if(s_pid == 0) // son
        {
            exfl = execl(argv[1], argv[1], argv[2], NULL);
            if(exfl == -1)
            {
                printf("process fail1\n");
                abort();
            }
        }
        else
        {
            exit(-1); // неудача 1 fork()
        } ////////////////////////////////////////////////////////fork end
    }




    else // Последовательно
    {
        //////////////////////////////////////////////////////////fork1 start
        if((s_pid = fork()) < 0) //dad
        {
            exit(-1); // неудача 1 fork()
        }
        else if(s_pid == 0) // son
        {
            exfl = execl(argv[1], argv[1], argv[2], NULL);
            if(exfl == -1)
            {
                printf("process fail1\n");
                abort();
            }
        }
        else
        {
            //dad
            wait(&err);
            printf("Process 1 ended up with code: %5d ", err);
            reason_end(err);
            if ((s_pid2 = fork()) < 0) //////////////////////////////////fork2 start
            {
                exit(-1);
            }
            else if (s_pid2 == 0) // son 2
            {
                exfl = execl(argv[3], argv[3], argv[4], NULL);
                if(exfl == -1)
                {
                    printf("process fail2\n");
                    abort();
                }
            }
            else
            {
                wait(&err);
                printf("Process 2 ended up with code: %5d ", err);
                reason_end(err);
            }
            ///////////////////////////////////////////////////////////fork2 end
            
        }
        ///////////////////////////////////////////////////////////fork1 end
        
    }
    return 0;
}