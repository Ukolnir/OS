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

#define SEM_NAME "POSIX8sem"
#define CNT 7  //Количество процессов


int my_random(){
    int A = 1, B = 100;
    return A + rand() % ((B + 1) - A);
}

typedef struct {
    int num; //переданное число 
    int get; //число получено       
} dataStr;   
  
void parent(dataStr* p, sem_t* sem){
    sem_wait (sem);
    
    p[0].num = my_random();
    printf("p.num = %d\n", p[0].num);
        
    //Запуск второго процесса
    p[1].num = p[0].num + 1;    
    p[1].get = 1;
    sem_post (sem);
    return;
}

void child(dataStr* p, int i, sem_t* sem){
    while(1)  //Ждем сигнала, когда процесс получит число
        if (p[i].get==1)
            break;
    sem_wait (sem);    
    
    if (i != CNT-1){
        p[i+1].num = p[i].num + 1;
        p[i+1].get = 1; //Сигнализируем следующему процессу, что он скоро начнет работу               
    }
    else{
        p[0].num = p[i].num+1;
        p[0].get = 1;
    }
    printf ("  Child(%d) new value of p.num = %d.\n", i, p[i].num);
   sem_post (sem); 
   return;
}

                
int main()
{
  srand(time(NULL));
  int i,j,k;  
  key_t key;  
  int shmid;
  pid_t pid;  
  dataStr *p;
  sem_t *sem;   
  unsigned int val_sem;
  
  if((key = ftok("POSIX8.c",0)) < 0){
      printf("Can\'t generate key\n");
      exit(EXIT_FAILURE);
  }
  
  shmid = shmget(key, CNT*sizeof(dataStr),0644|IPC_CREAT);
  if(shmid < 0)
    exit(EXIT_FAILURE);
  
  p = (dataStr *)shmat(shmid, NULL, 0);      
  for(i = 0; i < CNT; ++i){
    p[i].num = 0;
    p[i].get = 0;
  }

    sem_unlink ("pSem");
    sem_close(sem);
    val_sem = 1;
    sem = sem_open("pSem", O_CREAT | O_EXCL, 0644, val_sem);
      
  for (i = 0; i < CNT; ++i){  
    pid = fork();  
    if (pid < 0){
        sem_unlink("pSem");
        sem_close(sem);
        printf ("Fork error.\n");        
        exit(EXIT_FAILURE);
    }
    else
        if(pid == 0)
            break;    
  }

    /*-------------   PARENT PROCESS   ---------------*/
     if (pid != 0){
        parent(p, sem);
        while (pid = waitpid (-1, NULL, 0)){
            if (errno == ECHILD)
                break;
        }
        
        printf("result: p[0].num = %d\n",p[0].num);
        //printf ("\nParent: All children have exited.\n");

        shmdt (p);
        shmctl (shmid, IPC_RMID, 0);
        sem_unlink ("pSem");
        sem_close(sem);
        exit (EXIT_SUCCESS);
    }
    
    /*--------------   CHILD PROCESS   ---------------*/
    else{
        child(p, i, sem);
        exit (EXIT_SUCCESS);
    }
}
