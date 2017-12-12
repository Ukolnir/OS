#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
//gcc -std=c99 POSIX2.c -lpthread -o pos2.exe
#define SIZE 1000
#define N 7

typedef struct {
int** ar;
int start, end;
char result;
} dataStr;

void* threadF(void* thread_data){

    dataStr *data = (dataStr*) thread_data;

    int i,j;
    int check;
	for (int k = data->start; k < data->end; ++k){
		if (k<=0){
			i = abs(k);
			j = 0;
		}
		else{
			i = 0;
			j = k;
		}
		check = data->ar[i][j];
		++i; ++j;
		while((i < SIZE) && (j<SIZE)){
        	if(check!=data->ar[i][j]){
        		data->result = 0;
				return NULL;
			}
			++i; ++j;
		}
	}
	return NULL;
}

void matr(int** arr){
	int i, j;
	for (i = 0; i < SIZE; ++i){
		arr[i] = (int*)malloc(SIZE * sizeof(int));
		for (j = 0; j < SIZE; ++j)
			arr[i][j] = i + j;
	}
}

void matrRight(int** arr){
	int i, j;
	for (i = 0; i < SIZE; ++i){
		arr[i] = (int*)malloc(SIZE * sizeof(int));
		for (j = 0; j < SIZE; ++j)
			arr[i][j] = j-i;
	}
}

int main(){
	double time;
	
	pthread_t* threads = (pthread_t*) malloc(N * sizeof(pthread_t));

	dataStr* threadData = (dataStr*) malloc(N * sizeof(dataStr));   
        
       
	int cntDig = (SIZE - 1)*2 + 1;
	int lB = 1 - SIZE;
    int uB = cntDig - SIZE;
    int step = cntDig / N;

	int** matrix = (int**)malloc(SIZE * sizeof(int*));
	matr(matrix);							
		//--------------
	 clock_t start = clock();
	for(int q = 0; q < N; ++q){		
		if (q!=N-1){
			threadData[q].start = lB;
			threadData[q].end = lB+=step;
			threadData[q].ar = matrix;
			threadData[q].result = 1;
		}
		else{
			threadData[q].start = lB;
			threadData[q].end = uB;
			threadData[q].ar = matrix;
			threadData[q].result = 1;
		}	
		pthread_create(&(threads[q]), NULL, threadF, &threadData[q]);
	}
	
	for(int q = 0; q < N; ++q)
		pthread_join(threads[q], NULL);
	
	int result = 1;	
	for(int q = 0; q < N; ++q)
		result *= threadData[q].result;
 	printf("Result: %d \n", result);
	
	free(threads);
	free(threadData);
	for(int i = 0; i < N; ++i)
		free(matrix[i]);
	free(matrix);

	time = (clock() - start); 
	printf("Time of work: %f \n", time);

   return 0;    
}
