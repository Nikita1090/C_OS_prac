#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>	
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
int main(int argc, char* argv[]) {

    struct stat stat1;
    struct stat stat2;
    int buf,res, k=0, i=0, flag=0;
    int d;
    int fd = open(argv[1], O_RDWR);
    int output = open(argv[2], O_WRONLY);
    if((fd < 0) || (output < 0))
    {
        printf("Ошибка открытия\n");
        return 0;
    }
   
    // Проверяем, указывают ли оба имени на один и тот же файл
    fstat(fd, &stat1);
    fstat(output, &stat2); // получаем запись дескриптора
    if ((stat1.st_dev == stat2.st_dev) && (stat1.st_ino == stat2.st_ino)) // сравниваем данные дескрипторов
    {
        printf("Один файл\n");
        close(fd);
        close(output);
        return 0;
    } 
    close(output);
    // к - граница между отр и пол 
    while((res = read(fd, &buf, sizeof(int))) > 0){ //записываем отрицательные в начало
        i++;
        if (buf>=0 && flag==2) {//положительное после отрицательных -  i-1 граница
            k=i-1;
            flag=1;
        }
        if (buf<0 && flag==1) {//меняем местами положительное с отрицательным и двигаем границу
            lseek(fd, k*4, SEEK_SET);
            read(fd, &d, 4);
            lseek(fd, k*4, SEEK_SET);
            write(fd, &buf, 4);
            lseek(fd, (i-1)*4, SEEK_SET);
            write(fd, &d, 4);
            k++;
        }
        if (flag==0){
            if (buf<0){
                flag=2;//ищем первое положительное
            }
            else {
            flag=1;//ищем первое  отр 
            }
        }
    }
    lseek(fd, 0, SEEK_SET);
    FILE* f=fopen(argv[2],"w");
    while((res = read(fd, &buf, sizeof(int))) > 0) {
        fprintf(f, "%d ", buf);
    }
    close(fd);
    close(f);
    return 0;
    }
