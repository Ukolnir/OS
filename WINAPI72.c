#include <windows.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <malloc.h>
#include <time.h>

#define SIZE 6
#define N 2

typedef struct {
    int** arr;
    int lB, uB;     //границы диапазона. Элементы в столбце uB идут в мьютекс
    int n;         //номер потока + номер мьютекса
} dataStr;

HANDLE mutex[N];          
SYNCHRONIZATION_BARRIER barrier;


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
    ReleaseMutex(&mutex[n]);
    EnterSynchronizationBarrier(&barrier, 
    SYNCHRONIZATION_BARRIER_FLAGS_BLOCK_ONLY);
    WaitForSingleObject(&mutex[(n+1)%N], INFINITE);
    for (i = 0; i < SIZE; ++i)
        matrix[i][(uB+1)%SIZE] = tmp[i];
    ReleaseMutex(&mutex[(n+1)%N]);
}

DWORD WINAPI threadF(LPVOID thread_data){
    dataStr* data = (dataStr*) thread_data;
    shiftDr(data->arr, data->lB, data->uB);
    WaitForSingleObject(&mutex[data->n], INFINITE);
    shiftRr(data->arr, data->lB, data->uB, data->n);
    return 0;
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

    HANDLE threads[N];
    dataStr threadData [N];
    int** matrix = (int**)malloc(SIZE * sizeof(int*));
    makeMatr(matrix);
    int q,p;
    for(q = 0; q< SIZE; ++q){
        for(p = 0; p <SIZE; ++p)
            printf("%d ",matrix[q][p]);
        printf("\n");
	}
    InitializeSynchronizationBarrier(&barrier, N, 0);
    int i,j;
    for(i = 0; i<N; ++i)
        mutex[i] = CreateMutex(NULL, FALSE, NULL); //Защищает первый столбик своего диапазона
    int lB = 0, uB = SIZE-1;
    int step = uB / N;

    clock_t start = clock();

	for(j = 0; j < N; ++j){
		threadData[j].lB = lB;
		if (j!=N-1){
			threadData[j].uB = lB+=step;
			++lB;
		}
		else
            threadData[j].uB = uB;
		threadData[j].n = j;
		threadData[j].arr = matrix;
	}

    for(j = 0; j < N; ++j)
        threads[j] = CreateThread(NULL,0,threadF, &threadData[j],0,NULL);
    WaitForMultipleObjects(N,  threads, TRUE, INFINITE);

    time = (clock() - start)/CLOCKS_PER_SEC;
    printf("\n Time of work: %f \n", time);
    DeleteSynchronizationBarrier(&barrier);

    printf("\n\n");

   for(q = 0; q< SIZE; ++q){
        for(p = 0; p <SIZE; ++p)
            printf("%d ",matrix[q][p]);
        printf("\n");
	}

    for(i = 0; i < SIZE; ++i)
		free(matrix[i]);
	free(matrix);

   return 0;
}