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
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>          /* printf()                 */
#include <stdlib.h>         /* exit(), malloc(), free() */
#include <errno.h>          /* errno, ECHILD            */

#define SEM_NAME "POSIX9sem"

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
    
    sem_t *sem;   
    unsigned int val_sem;
  
    printf("Before open pipe \n");
    if (pipe(fd1) == -1){
        perror("Can't open pipe.");
        exit(EXIT_FAILURE);
    }
    printf("before record to pipe \n");    
    
    sem_unlink ("pSem");
    sem_close(sem);
    val_sem = 1;
    sem = sem_open("pSem", O_CREAT | O_EXCL, 0644, val_sem);
        
    fpid = fork();
    if (fpid == -1){
        perror("Can't fork first children pipe.");
        exit(EXIT_FAILURE);
    }
    
    
    if(fpid){
        sem_wait(sem);        
        for(int i = 0; i < 5; ++i){
            printf("Enter %d strings: ", i);
            scanf("%s", str[i]);
            len[i] = strlen(str[i]);
            printf("len[%d]: %d \n", i, len[i]);        
        }
        
        for(int i = 0; i < 5;++i){
            write(fd1[1], &len[i], sizeof(size_t));
            write(fd1[1], &str[i], len[i]*sizeof(char));
            printf("Record to pipe: %d \n", i);
        }
        close(fd1[0]);
    
        sem_post(sem);    
    
        while (fpid = waitpid (-1, NULL, 0))
            if (errno == ECHILD)
                break;
        sem_unlink ("pSem");
        sem_close(sem);
        exit (EXIT_SUCCESS);
    }    
    else{
        printf("Child works \n");
        char str1[3];
        size_t l;
        sem_wait(sem);
        for(int i = 0; i<3; ++i){
            read(fd1[0],&l,sizeof(size_t));
            printf("Read len: %d \n",l);
            read(fd1[0],&str1[i], l*sizeof(char));
            printf("Read str1[%d] \n",i);       
        }
        close(fd1[1]);
        
        sem_post(sem);
        exit (EXIT_SUCCESS);

    }
}











