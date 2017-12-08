#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define CNT 2

int main()
{
  int *array;
  int shmid;
  char pathname[] = "POSIX8.c";
  key_t key;
  if((key = ftok(pathname,0)) < 0){
      printf("Can\'t generate key\n");
      exit(EXIT_FAILURE);
  }
  
  shmid = shmget(key, CNT*sizeof(int),0666|IPC_CREAT|IPC_EXCL);
  if(shmid < 0)
    exit(EXIT_FAILURE);
  
  if((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)){
    printf("Can't attach shared memory\n");
    exit(EXIT_FAILURE);
  }  
    
    
    
    
    
    
    
    
    
    
    
  pid_t pid;
  int rv;
  int i;
  for (i = 0; i < CNT; ++i){
    pid = fork();  
    if (pid < 0)
        exit(EXIT_FAILURE);
    else
        if(pid == 0)
            break;    
  }  
  
  
                                                                                
                                                                                
                                                                                
                                                                                
                                 
  
  
  
  
  
  
}
