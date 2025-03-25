#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>	
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

char* intRfl_to_str(int n) // функция с семинара, выводит строчку вида rwx
{
    char* temp = "rwxrwxrwx";
    int len = strlen(temp);
    char* str = calloc(10, sizeof(char));
    str[9] = '\0';
    int mask = 1 << 8;
    for(int i = 0; i<len; i++)
    {
        if(mask & n)
        {
            str[i] = temp[i];
        }
        else
        {
            str[i] = '-';
        }
        mask >>= 1;
    }
    return str;
}

void dircheck(char* Name, int level)
{
    printf("\n"); // нужно для красоты при рекурсии (функция рекурсивна)
    DIR* d = opendir(Name); // открываем диррикторию
    struct dirent *s;
    struct stat stat1;
    char* str; // различные буферы строк
    char* buf;
    char* bufname; // буфер для адреса или имени нужного файла
    int lensym;
    while((s = readdir(d)) != NULL)
    {
        bufname = calloc(strlen(Name) + strlen(s->d_name) + 1 + 1, sizeof(char));
        strcpy(bufname, Name);
        bufname[strlen(Name)] = '/';
        strcat(bufname, s->d_name); // файлы из каталогов вызываются из домашнего каталога (почему? зачем?)
        lstat(bufname, &stat1); // получаем запись stat
        if(S_ISREG(stat1.st_mode))
        {
            if ((stat1.st_mode & (256 >> 2)) && (stat1.st_mode & (256 >> 5))) // 001001000
            {
                if(level>0) // для красивого отображения в подкаталогах
                {
                /*for(int i = 0; i<level; i++)
                    printf("\t");*/
                printf("∟");
                }
                printf("|%16s| ", s->d_name); // имя rwx идентификатор 
                str = intRfl_to_str(stat1.st_mode); 
                printf("%10s ", str);
                printf("%10d ", (int) stat1.st_ino);
                free(str);
                printf("\n");
            }
        }
        else if(S_ISDIR(stat1.st_mode))
        {
            //printf(  "DIR    ");
            if((strcmp(s->d_name, "..") != 0) && (strcmp(s->d_name, ".") != 0)) // игнорим родительский и собственный каталог(иначе с рекурсией до связи...)
            {
            if(level>0)
            {
            printf("∟"); // зацените какой уголочек нашёл ☻
            }
            printf("|%16s| ", s->d_name);
            str = intRfl_to_str(stat1.st_mode); // тут всё как и выше
            printf("%10s ", str);
            printf("%10d ", (int) stat1.st_ino);
            free(str);
            // а вот тут рекуривный преход
            dircheck(bufname, level + 1);
            } 
        }
        else if(S_ISLNK(stat1.st_mode))
        {
            if(level>0)
            {
            printf("∟");
            }
            printf("|%16s| ", s->d_name);
            str = intRfl_to_str(stat1.st_mode); // аналогичный вывод инфы
            printf("%10s ", str);
            printf("%10d ", (int) stat1.st_ino);
            free(str);
            buf = calloc(100, 1);
            lensym = readlink(bufname, buf, 30); // смотрим на что указывает ссылка
            if (lensym < 1)
            {
                free(buf);
                free(bufname);
                exit(-1);
            }
            printf(" link:%s", buf);
            free(buf); 
            printf("\n");  
        }
        free(bufname); // не теряем строку адрес
    }
    closedir(d);

}


int main(int argc, char *argv[])
{
    if(argc != 2)
        return -1; // а больших проверок мне и не надо!
    dircheck(argv[1], 0);
    return 0;
}