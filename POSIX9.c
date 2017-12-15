#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <malloc.h>
#include <time.h>
#include <pthread.h>
#include <stddef.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <fcntl.h>  
#include <string.h>


//Добавить семафор для синхронизации, 0 - писать в канал, 1 - читать из канала
int fd1[2], fd2[2]; //Дескрипторы для каналов

char* str_replace(char* dst, int num, const char* str, const char* orig, const char* rep){
    const char* ptr;
    size_t len1 = strlen(orig);
    size_t len2 = strlen(rep);
    char*  tmp  = dst;
   
    num -= 1;
    while((ptr = strstr(str, orig)) != NULL){
        num -= (ptr - str) + len2;
        if(num < 1)
            break;

        strncpy(dst, str, (size_t)(ptr - str));
        dst += ptr - str;
        strncpy(dst, rep, len2);
        dst += len2;
        str  = ptr + len1;
    }

    for(; (*dst = *str) && (num > 0); --num){
        ++dst;
        ++str;
    }
    return tmp;
}

int main(){
    char str[5][20];
    pid_t fpid;
    size_t len[5];    
    
    for(int i = 0; i < 5; ++i){
        printf("Enter %d strings: ", i);
        scanf("%s", str[i]);
        len[i] = strlen(str[i]);
        printf("len[%d]: %d \n", i, len[i]);        
    }
    printf("Before open pipe \n");
    if (pipe(fd1) == -1){
        perror("Can't open pipe.");
        exit(EXIT_FAILURE);
    }
    printf("before record to pipe \n");    
    
    for(int i = 0; i < 5;++i){
        write(fd1[1], &len[i], sizeof(size_t));
        write(fd1[1], &str[i], len[i]*sizeof(char));
        printf("Record to pipe: %d \n", i);
    }
    close(fd1[0]);
    
    fpid = fork();
    if (fpid == -1){
        perror("Can't fork first children pipe.");
        exit(EXIT_FAILURE);
    }
    
    if(fpid){
        while (fpid = waitpid (-1, NULL, 0))
            if (errno == ECHILD)
                break;
        exit (EXIT_SUCCESS);
    }    
    else{
        printf("Child works \n");
        char str1[3];
        size_t l;
        for(int i = 0; i<3; ++i){
            read(fd1[0],&l,sizeof(size_t));  //791621423???????
            printf("Read len: %d \n",l);
            read(fd1[0],&str1[i], l*sizeof(char));
            printf("Read str1[%d] \n",i);       
        }
        close(fd1[1]);
        
        exit (EXIT_SUCCESS);

    }
}











