#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>

struct Messageint
{
    long mtype;
    int Dataint;
};

struct Messagechar
{
    long mtype;
    int Datachar;
};

int space_count(char* str)
{
    if (str == NULL)
    {
        return -1;
    }
    int res = 0;
    for(int i = 0; i<strlen(str); i++)
    {
        if(str[i] == ' ')
        {
            res++;
        }
    }
    //fprintf(stderr, "count%d\n", res);
    return res;
}


int main(void)
{
    
    pid_t pids1;
    pid_t pids2;
    FILE* file = fopen("testf", "r");
    int shmid;
    
    if((pids1 = fork()) == 0) // s1
    {
        fclose(file);
        sleep(1);
        key_t keymem = ftok(".", 't');
        struct Messagechar get_sig;
        get_sig.Datachar = 1;
        struct Messageint result_back;
        result_back.mtype = 4;
        int d_mess_s1 = msgget(keymem, 0666);
        msgrcv(d_mess_s1, (struct Messagechar*) &get_sig, 1, sizeof(char), 0);
        shmid = shmget(keymem, 100, 0666);
        //printf("BBBB\n");
        char* shareStr1 = shmat(shmid, NULL, SHM_RDONLY);
        while (get_sig.Datachar)
        {
            result_back.Dataint = strlen(shareStr1)-1;
            msgsnd(d_mess_s1, (struct Messageint*)(&result_back), 1, 0);
            msgrcv(d_mess_s1, (struct Messagechar*) (&get_sig), 1, sizeof(char), 0);
        }
        //printf("Sinyla1 vmer\n");
        msgctl(d_mess_s1, IPC_RMID, NULL);
        shmdt(shareStr1);
        shmctl(shmid, IPC_RMID, NULL);
        exit(0);
    }
    if(pids1 < 0)
    {
        abort();
    }

    if((pids2 = fork()) == 0) // s2
    {
        fclose(file);
        sleep(1);
        key_t keymem = ftok(".", 't');
        key_t keymem2 = ftok(".", 'y');
        struct Messagechar get_sig;
        get_sig.Datachar = 1;
        struct Messageint result_back;
        result_back.mtype = 4;
        int d_mess_s2 = msgget(keymem2, 0666);
        //printf("AAAA\n");
        msgrcv(d_mess_s2, (struct Messagechar*) &get_sig, 1, sizeof(char), 0);
        shmid = shmget(keymem, 100, 0666);
        char* shareStr1 = shmat(shmid, NULL, SHM_RDONLY);
        while (get_sig.Datachar)
        {
            result_back.Dataint = space_count(shareStr1);
            msgsnd(d_mess_s2, (struct Messageint*)(&result_back), 1, 0);
            msgrcv(d_mess_s2, (struct Messagechar*) &get_sig, 1, sizeof(char), 0);
        }
        msgctl(d_mess_s2, IPC_RMID, NULL);
        shmdt(shareStr1);
        shmctl(shmid, IPC_RMID, NULL);
        //printf("Sinyla2 vmer\n");
        exit(0);
        /*
        fclose(file);
        sleep(2);
        key_t keymem = ftok(".", 'b');
        key_t keymem2 = ftok(".", 'c');
        struct Messagechar get_sig;
        get_sig.Datachar = 1;
        struct Messageint result_back;
        result_back.mtype = 4;
        int d_mess_s2 = msgget(keymem2, 0666);
        msgrcv(d_mess_s2, (struct Messagechar*) &get_sig, 1, sizeof(char), 0);
        printf("AAAA\n");
        shmid = shmget(keymem, 100, 0666);
        char* shareStr1 = shmat(shmid, NULL, SHM_RDONLY);
        while (get_sig.Datachar)
        {
            printf("AAAA\n");
            result_back.Dataint = space_count(shareStr1);
            msgsnd(d_mess_s2, (struct Messageint*)(&result_back), 1, 0);
            msgrcv(d_mess_s2, (struct Messagechar*) &get_sig, 1, sizeof(char), 0);
        }
        
        exit(0);*/
    }
    if(pids1 < 0)
    {
        abort();
    }
    //FATHER
    key_t keymem = ftok(".", 't');
    key_t keymem2 = ftok(".", 'y');
    if ((shmid = shmget(keymem, 100, 0666 | IPC_CREAT | IPC_EXCL)) <= 0)
    {
        fprintf(stderr, "MEM ERR\n");
        abort();
    }
    char line[100];
    char* shareStr = shmat(shmid, NULL, 0);
    int d_mess_s1 = msgget(keymem, 0666 | IPC_CREAT | IPC_EXCL);
    int d_mess_s2 = msgget(keymem2, 0666 | IPC_CREAT | IPC_EXCL);
    struct Messagechar obratno;
    struct Messageint result;
    obratno.mtype = 1;
    obratno.Datachar = 1;

    while(fgets(line, sizeof(line), file) != NULL)
    {
        strcpy(shareStr, line);
        msgsnd(d_mess_s1, (struct Messagechar*)(&obratno), 1, 0);
        msgrcv(d_mess_s1, (struct Messageint*) &result, 1, sizeof(int), 0);
        printf("|%s|Длина строки: %d", line, result.Dataint);
        msgsnd(d_mess_s2, (struct Messagechar*)(&obratno), 1, 0);
        msgrcv(d_mess_s2, (struct Messageint*) &result, 1, sizeof(int), 0);
        printf(" Количество пробелов: %d\n", result.Dataint);
    }
    obratno.Datachar = 0;
    msgsnd(d_mess_s1, (struct Messagechar*)(&obratno), 1, 0);
    msgsnd(d_mess_s2, (struct Messagechar*)(&obratno), 1, 0);
    shmdt(shareStr); // отвязка должна быть в каждом процессе
    shmctl(shmid, IPC_RMID, NULL); // закрытие
    msgctl(d_mess_s1, IPC_RMID, NULL);
    msgctl(d_mess_s2, IPC_RMID, NULL);
    fclose(file);
    //printf("Batia yshel za pivkom\n");
    exit(0);
}