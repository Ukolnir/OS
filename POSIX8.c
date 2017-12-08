#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define CNT 2

typedef struct {    
    int n;  //number of process
    int data; 
    //нужно ли еще одно поле работает или нет?       
} dataStr;









int main()
{
  srand(time(NULL));
  int i,j,k;  
  key_t key;  
  int shmid;
  pid_t pid;  
  info *p; //shared vaiable array?
  unsigned int val_sem; //semaphore value
  
  if((key = ftok("POSIX8.c",0/**/)) < 0){  //4?
      printf("Can\'t generate key\n");
      exit(EXIT_FAILURE);
  }
  
  shmid = shmget(key, CNT*sizeof(info),0644|IPC_CREAT);
  if(shmid < 0)
    exit(EXIT_FAILURE);
  
  p = (info *)shmat(shmid, NULL, 0);    
    
  for(i = 0; i < CNT; ++i){
    p[i].n = 0;
    p[i].data = 0;
  }

  p[0].data = 1; //???  
      
  for (i = 1; i < CNT; ++i){  //change, i=0
    pid = fork();  
    if (pid < 0){
        //sem_unlick("pSem");
        //sem_close(sem);        
        exit(EXIT_FAILURE);
    }
    else
        if(pid == 0)
            break;    
  }  
  
  
                                                                                
                                                                                
                                                                                
                                                                                
                                 
  
  
  
  
  
  
}
