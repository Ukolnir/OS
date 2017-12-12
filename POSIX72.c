#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
//gcc -std=c99 POSIX72.c -lpthread -o pos2.exe
#define SIZE 1000

#define N 10

typedef struct {
    int** arr;
    int lB, uB;     //границы диапазона. Элементы в столбце uB идут в мьютекс
    int n;         //номер потока + номер мьютекса
} dataStr;

pthread_mutex_t mutex[N];       
pthread_barrier_t barrier;


void shiftDr(int** arr, int lB, int uB){
    int i,j,t;
    for(i = lB; i <= uB ; ++i){
        t = arr[SIZE-1][i];
        for(j = SIZE-1; j > 0; --j)
            arr[j][i] = arr[j-1][i];
        arr[j][i] = t;
    }
}

void shiftRr(int** matrix, int lB, int uB, int n){
	int i,j,t;
    int* tmp = (int*)malloc(SIZE * sizeof(int)); 
    for(i = 0; i < SIZE; ++i){
        tmp[i] = matrix[i][uB];
        for(j = uB; j>lB; --j)
            matrix[i][j] = matrix[i][j-1];
    }
    pthread_mutex_unlock(&mutex[n]);
	pthread_barrier_wait(&barrier);
    pthread_mutex_lock(&mutex[(n+1)%N]);
    for (i = 0; i < SIZE; ++i)
        matrix[i][(uB+1)%SIZE] = tmp[i];
     pthread_mutex_unlock(&mutex[(n+1)%N]);
}

void* threadF(void* thread_data){
    dataStr *data = (dataStr*) thread_data;
    shiftDr(data->arr, data->lB, data->uB);
    pthread_mutex_lock(&mutex[data->n]);
    shiftRr(data->arr, data->lB, data->uB, data->n);
    return NULL;
}

void makeMatr(int** arr){
    int i, j;
    for (i = 0; i < SIZE; ++i){
        arr[i] = (int*)malloc(SIZE * sizeof(int));
	for (j = 0; j < SIZE; ++j)
		arr[i][j] = (j+i) % 10;
	}
}

int main(){
    double time;

    pthread_t* threads = (pthread_t*) malloc(N * sizeof(pthread_t));

    dataStr* threadData = (dataStr*) malloc(N * sizeof(dataStr));

    int** matrix = (int**)malloc(SIZE * sizeof(int*));
    makeMatr(matrix);

    /*for(int i = 0; i< SIZE; ++i){
        for(int j = 0; j <SIZE; ++j)
            printf("%d ",matrix[i][j]);
        printf("\n");
	}*/
    
    pthread_barrier_init(&barrier, NULL, N);    
    
    for(int i = 0; i<N; ++i)
        pthread_mutex_init(&mutex[i], NULL); //Защищает первый столбик своего диапазона

    int lB = 0, uB = SIZE-1;
    int step = uB / N;
    
    clock_t start = clock();

    for(int q = 0; q < N; ++q){
        threadData[q].lB = lB;
	if (q!=N-1){
		threadData[q].uB = lB+=step;
		++lB;
	}
	else
		threadData[q].uB = uB;
	threadData[q].n = q;	
	threadData[q].arr = matrix;
	pthread_create(&(threads[q]), NULL, threadF, &threadData[q]);
    }

    for(int q = 0; q < N; ++q)            
	pthread_join(threads[q], NULL);
    time = (clock() - start)/CLOCKS_PER_SEC;
    printf("\n Time of work: %f \n", time);

    pthread_barrier_destroy(&barrier);
    free(threads);
    free(threadData);

    printf("\n\n");

   /* for (int i = 0; i < SIZE; ++i){
        for(int j = 0; j<SIZE; ++j)
            printf("%d ",matrix[i][j]);
        printf("\n");
    }*/

    for(int i = 0; i < SIZE; ++i)
		free(matrix[i]);
	free(matrix);
   return 0;
}





