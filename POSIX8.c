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
#define CNT 2


int my_random(){
    int A = 1, B = 100;
    return A + rand() % ((B + 1) - A);
}

typedef struct {
    int num; //переданное число 
    int get; //число получено       
} dataStr;   //info
  
/*---------------------PART 2-----------------------------------------------*/
//первый процесс генерирует последовательность случайных чисел, 
//которые передаёт по одному второму процессу
void first(dataStr* p, sem_t* sem){
    sem_wait (sem);
    //Установка нового числа 0-му процессу
    p[0].num = my_random();
    //printf("p.num = %d\n", p[0].num);
    p[0].get = 0;
        
    //Транслирование числа 1-му процессу
    p[1].num = p[0].num + 1;    
    p[1].get = 1;
    sem_post (sem); 
    return;
//Сделать условие для проверки получил значение первый процесс или сгенерировал его?      
}

//Второй процесс увеличивает каждое получаемое число на 1 
//и передаёт дальше. 
void others_p(dataStr* p, int i, sem_t* sem){
    sem_wait (sem);
    //Работа с процессами, у которых индекс от 1 до n-2 включительно
    if (i != CNT-1){
    //Говорим следующему процессу, что он может начинать работать
    p[i+1].num = p[i].num + 1;
    p[i+1].get = 1;                
    }
    else{
     //Показываем, что текущий процесс может принимать следующее число
     p[0].num = p[i].num+1;
     p[0].get = 1;
    }
    //sem_post (sem); 
    //return;
   sem_post (sem); 
   //return ;
}



//Выводим индекс текущего процесса и число
  //                  printf ("  Child(%d) new value of p.num = %d.\n", i, p[i].num);
                
/*----------------------------END PART 2 ---------------------------*/


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

   //очистка семафора из ядра; закрытие семафора
    sem_unlink ("pSem");
    sem_close(sem);
    val_sem = 1;

    
    sem = sem_open("pSem", O_CREAT | O_EXCL, 0644, val_sem);
      
  for (i = 0; i < CNT; ++i){  
    pid = fork();  
    if (pid < 0){
        sem_unlick("pSem");
        sem_close(sem);
        printf ("Fork error.\n");        
        exit(EXIT_FAILURE);
    }
    else
        if(pid == 0)
            break;    
  }

    /******************   PARENT PROCESS   ****************/
    if (pid != 0){
        
        first(p, sem);
        while (pid = waitpid (-1, NULL, 0)){
            if (errno == ECHILD)
                break;
        }
        printf ("\nParent: All children have exited.\n");

        /* shared memory detach */
        shmdt (p);
        shmctl (shmid, IPC_RMID, 0);
        
        /* cleanup semaphores */
        sem_unlink ("pSem");
        sem_close(sem);
        /* unlink prevents the semaphore existing forever */
        /* if a crash occurs during the execution         */
        exit (0);
    }
    
    /******************   CHILD PROCESS   *****************/
    else{
        others_p(p, i, sem);
        exit (0);
    }
}
