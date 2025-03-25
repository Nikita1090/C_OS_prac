#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>	
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char ** argv)
{
    struct stat stat_1; // для проверки на эквивалентность файла
    struct stat stat_2;
    int fd1 = open(argv[1], O_RDWR);
    int fd2 = open(argv[2], O_WRONLY | O_CREAT, 0600);
    if (argc < 3)
    {
        fprintf(stderr, "Err few arguments\n");
        return -1;
    }
    if((fd1 < 0) || (fd2 < 0))
    {
        fprintf(stderr, "Err open()\n");
        return -1;
    }
    fstat(fd1, &stat_1);
    fstat(fd2, &stat_2); // получаем запись дескриптора
    if ((stat_1.st_dev == stat_2.st_dev) && (stat_1.st_ino == stat_2.st_ino)) // сравниваем данные дескрипторов
    {
        fprintf(stderr, "Один и тот же файл\n");
        if((close(fd1) || close(fd2)) == 0)
        {
            return -1;
        }
        return 0;
    } 
    lseek(fd2, 0, SEEK_SET); // ставим указатель во 2 файле в начало
    ssize_t ret;
    int buf, buf2;
    int fl = 0;
    int i = 0;
    int j = 0;
    while((ret = read(fd1, &buf, sizeof(int))) > 0)
    { 
        //переносим отрицательные в начало
        i++;
        if (buf>=0 && fl==2) 
        {                    
            j = i - 1;
            fl = 1;
        }
        if (buf<0 && fl==1) 
        {            //меняем положительное с отрицательным и двигаем границу
            lseek(fd1, j*4, SEEK_SET);
            read(fd1, &buf2, 4);
            lseek(fd1, j*4, SEEK_SET);
            write(fd1, &buf, 4);
            lseek(fd1, (i-1)*4, SEEK_SET);
            write(fd1, &buf2, 4);
            j++;
        }
        if (fl==0)
        {
            if (buf<0)
            {
                fl = 2;       //ищем первое положительное
            }
            else 
            {
                fl = 1;       //ищем первое отрицательное
            }
        }
    }
    if(lseek(fd1, 0, SEEK_SET) < 0) // ещё раз бежим по файлу
    {
        fprintf(stderr, "Ошибка смещения\n");
        return -1;
    }
    char * str = calloc(1, 10); // просто буфер для числа в ввиде текста
    while((ret = read(fd1, &buf, sizeof(int))) > 0)
    {
        snprintf(str, 10, "%d ", buf); // аналогично
        ret = write(fd2, str, strlen(str));
        if(ret < 0)
        {
            fprintf(stderr, "Ошибка записи\n");
            return -1;
        }
    }
    free(str);
    if(close(fd1) != 0) // аккуратно закрываем
    {
        return -1;
    }
    if(close(fd2) != 0)
    {
        return -1;
    }
    return 0;
}