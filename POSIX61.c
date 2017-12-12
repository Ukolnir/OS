#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000

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

int diagonal (int** x){
	int cntDig = (SIZE-1)*2 + 1;    
	int lB = -SIZE+1;
	int uB = cntDig - SIZE;
	int i,j;
    int check;

	for(int k = lB; k < uB; ++k){
		if (k<=0){
			i = abs(k);
			j = 0;
		}
		else{
			i = 0;
			j = k;
		}
		check = x[i][j];
		++i; ++j;
		while((i < SIZE) && (j<SIZE)){
        	if(check!=x[i][j])
        		return 0;
			++i; ++j;
		}
	}
	return 1;
}

int main(){
    double time;
	clock_t start = clock();

	int** matrix = (int**)malloc(SIZE * sizeof(int*));
	matrRight(matrix);
	
	int result = diagonal(matrix);
   	printf("Result: %d \n",result);

    time = (clock() - start); 
	printf("Time of work: %f \n", time);
   
   return 0;    
}
