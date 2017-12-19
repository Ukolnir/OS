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
#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#define SEM_NAME "POSIX9sem"

int fd1[2]; //Дескриптор для первого канала

char *str_replace2(char *orig, char *rep, char *with) {
	char *result;
	char *ins; 
	char *tmp; 
	int len_rep; 
	int len_with;
	int len_front; 
	int count; 
	if (!orig || !rep)
		return NULL;
	len_rep = strlen(rep);
	if (len_rep == 0)
		return NULL; 
	if (!with)
		with = "";
	len_with = strlen(with);
	ins = orig;
	for (count = 0; tmp = strstr(ins, rep); ++count)
		ins = tmp + len_rep;  
	tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);
	if (!result)
		return NULL;
	while (count--) {
		ins = strstr(orig, rep);
		len_front = ins - orig;
		tmp = strncpy(tmp, orig, len_front) + len_front;
		tmp = strcpy(tmp, with) + len_with;
		orig += len_front + len_rep; 
	}
	strcpy(tmp, orig);
	return result;
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
		sem_wait(sem);
		printf("Child works \n");
		char str1[5][20];
		for(int i = 0; i < 5; ++i)
			for(int j = 0; j < 19; ++j)
				str1[i][j] = '\0';
		size_t l;

		for(int i = 0; i<5; ++i){
			read(fd1[0],&l,sizeof(size_t));
			printf("Read len: %d \n",l);
			read(fd1[0],&str1[i], l*sizeof(char));			
			puts(str1[i]);	
			printf("\n");
		}
		printf("Replace \n");
		char* tsss;
		tsss = str_replace2(str1[0], str1[1], str1[2]);
		close(fd1[1]);
		puts(tsss);
		sem_post(sem);
		size_t len1 = strlen(tsss);
		printf("Len replace str: %d \n",len1);
		char sss[40];
		size_t a;
		for(a = 0; a < len1; ++a)
			sss[a] = *(tsss+a);
		sss[len1] = '\0';		
		printf("After str: \n");
		puts(sss);	
			
		int fd2[2];
		if (pipe(fd2) == -1){
			perror("Can't open pipe2");
			exit(EXIT_FAILURE);
		}
		printf("Record to pipe2: %d \n", 0);
		write(fd2[1], &len1, sizeof(size_t)); 
		write(fd2[1], &sss, len1*sizeof(char));
		printf("Record to pipe2 close: %d \n", 0);
		pid_t spid = fork();
		if (spid == -1){
			perror("Can't fork first children pipe.");
			exit(EXIT_FAILURE);
		}
	
		if(spid){
			sem_wait(sem);
			printf("Child work1\n");
			size_t l1;
			for(int b = 3; b < 5;++b){
				l1 = strlen(str1[b]);
				write(fd2[1], &l1, sizeof(size_t));
				write(fd2[1], &str1[b], l1*sizeof(char));
				printf("Record to pipe2: %d \n", b+1);
			}
			close(fd2[0]);
			printf("end child\n");
			sem_post(sem);
			while (spid = waitpid (-1, NULL, 0))
			if (errno == ECHILD)
				break;
			exit (EXIT_SUCCESS);
		}
		else{
			sem_wait(sem);
			printf("Child child works \n");
			char str2[3][40];
			for(int i = 0; i < 3; ++i)
				for(int j = 0; j < 40; ++j)
					str2[i][j] = '\0';
			size_t l2;
			for(int i = 0; i<3; ++i){
				read(fd2[0],&l2,sizeof(size_t));
				printf("Read len: %d \n",l2);
				read(fd2[0],&str2[i], l2*sizeof(char));
				puts(str2[i]);	
				printf("\n");
			}
			printf("Replace \n");
			char* sss1;
			sss1 = str_replace2(str2[0], str2[1], str2[2]);
			puts(sss1);
			close(fd2[1]);
			sem_post(sem);
			exit (EXIT_SUCCESS);
		}
	}
}

