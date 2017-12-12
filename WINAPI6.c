#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <malloc.h>
#include <time.h>

#define SIZE 1000
#define N 7

void matrRight(int** arr){
	int i, j;
	for (i = 0; i < SIZE; ++i){
		arr[i] = (int*)malloc(SIZE * sizeof(int));
		for (j = 0; j < SIZE; ++j)
			arr[i][j] = j-i;
	}
}

void matr(int** arr){
	int i, j;
	for (i = 0; i < SIZE; ++i){
		arr[i] = (int*)malloc(SIZE * sizeof(int));
		for (j = 0; j < SIZE; ++j)
			arr[i][j] = i + j;
	}
}

typedef struct {
int** ar;
int start, end;
char result;
} dataStr;

void* threadF(void* thread_data){
    dataStr *data = (dataStr*) thread_data;

    int i,j;
    int check;
    int k;

	for (k = data->start; k < data->end; ++k){
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

DWORD WINAPI thread_proc(LPVOID pvParams){
	threadF(pvParams);
	return 0;
}

int main(){
	int i;
	HANDLE ahThreads[N - 1];
	dataStr threadData[N];
	int** matrix;
	int result = 1;

	double time;
	clock_t start = clock();

	matrix = (int**)malloc(SIZE * sizeof(int*));
	matr(matrix);

	int cntDig = (SIZE - 1)*2 + 1;
	int lB = 1 - SIZE;
    int uB = cntDig - SIZE;
    int step = cntDig / N;

	for (i = 0; i < N; ++i)
	{
		if (i!=N-1){
			threadData[i].start = lB;
			threadData[i].end = lB+=step;
			threadData[i].ar = matrix;
			threadData[i].result = 1;
		}
		else{
			threadData[i].start = lB;
			threadData[i].end = uB;
			threadData[i].ar = matrix;
			threadData[i].result = 1;
		}
		if (i < N - 1)
			ahThreads[i] = CreateThread(NULL, 0, thread_proc, &threadData[i], 0, NULL);
	}
	thread_proc(&threadData[N - 1]);
    WaitForMultipleObjects(N-1, ahThreads, TRUE, INFINITE);

	for (i = 0; i < N; ++i)
		result *= threadData[i].result;
	printf("Result: %d\n", result);

	for (i = 0; i < SIZE; ++i)
		free(matrix[i]);
	free(matrix);
    for (i = 0; i < N-1; ++i )
	     CloseHandle(ahThreads[i]);

	time = clock() - start;
	printf("Time of work: %f\n", time);

	//system("pause");
}
