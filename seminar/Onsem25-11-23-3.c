#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int bit_received = 0;
pid_t child1_pid;
pid_t child2_pid;

void sigusr1_handler(int signo) 
{
    bit_received = 0;
}

void sigusr2_handler(int signo) 
{
    bit_received = 1;
}

void alrm_handler(int signo) 
{
    
}
void F_alrm_handler(int signo) 
{
    kill(child2_pid, SIGALRM);
    //fprintf(stderr, "FatherAlrm\n");
}
void kill_handler(int signo) 
{
    //fprintf(stderr, "son1KILL\n");
    printf("\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) 
{
    if (argc != 2) 
    {
        fprintf(stderr, "Использование: %s <имя>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int file_fd = open(argv[1], O_RDONLY);
    if (file_fd == -1) 
    {
        perror("Ошибка открытия файла");
        exit(EXIT_FAILURE);
    }
    signal(SIGALRM, F_alrm_handler);
    child1_pid = fork();

    if (child1_pid == -1) 
    {
        perror("Ошибка создания первого сына");
        exit(EXIT_FAILURE);
    }

    if (child1_pid == 0) 
    { // Код для первого сына
        close(file_fd);
        signal(SIGIO, kill_handler);
        signal(SIGUSR1, sigusr1_handler);
        signal(SIGUSR2, sigusr2_handler);
        char byte_res = 0;
        char fl = 0;
        while (1) 
        {
            //fprintf(stderr, "son1S\n");
            if (fl) // объяснение ниже
            {
                kill(getppid(), SIGALRM); 
            }
            fl = 1;
            byte_res = 0;
            for(int i = 0; i < 8; i++)
            {
                pause();
                byte_res <<= 1;
                byte_res += bit_received;
                if(i != 7) // иначе в случае выхода из цикла сигнал может прийти до паузы и всё полетит
                    kill(getppid(), SIGALRM); 
            }
            printf("%c", byte_res);
        }

        exit(EXIT_SUCCESS);
    }


    child2_pid = fork();

    if (child2_pid == -1) 
    {
        perror("Ошибка создания второго сына");
        exit(EXIT_FAILURE);
    }

    if (child2_pid == 0) 
    { // Код для второго сына
        signal(SIGKILL, kill_handler);
        signal(SIGALRM, alrm_handler);
        while (1) 
        {
            //fprintf(stderr, "son2S\n");
            char byte1;
            ssize_t bytes_read = read(file_fd, &byte1, 1);
            if (bytes_read == -1) 
            {
                perror("Ошибка чтения из файла");
                exit(EXIT_FAILURE);
            }
            if (bytes_read == 0) 
            {
                //fprintf(stderr, "son2K\n");
                close(file_fd);
                kill(child1_pid, SIGIO);
                exit(EXIT_SUCCESS);
            }
            //fprintf(stderr, "son2: %d\n", byte1);
            unsigned mask = (1 << 7);
            
            for (int i = 0; i < 8; i++)
            {
                //(stderr, "son2FOR\n");
                if ((byte1 & mask) == 0) 
                {
                    kill(child1_pid, SIGUSR1);
                    //fprintf(stderr, "son2FOR1\n");
                    pause();
                    //fprintf(stderr, "son2FOR2\n");
                } 
                else
                {
                    //fprintf(stderr, "son2FOR1\n");
                    kill(child1_pid, SIGUSR2);
                    //fprintf(stderr, "son2FOR2\n");
                    pause();
                }
                mask >>= 1;
            }

        }

        close(file_fd);

        exit(EXIT_SUCCESS);
    }




    waitpid(child1_pid, NULL, 0);
    waitpid(child2_pid, NULL, 0);

    printf("Передача завершена. Главный процесс завершается.\n");

    close(file_fd);

    return 0;
}
