#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 3

void makeMatr(int** arr){
    int i, j;
    for (i = 0; i < SIZE; ++i){
        arr[i] = (int*)malloc(SIZE * sizeof(int));
	for (j = 0; j < SIZE; ++j)
		arr[i][j] = (j+i) % 10;
	}
}

void shiftD(int** arr){
    int i,j,t;
    for(i = 0; i < SIZE; ++i){
        t = arr[SIZE-1][i];
        for(j = SIZE-1; j > 0; --j)
            arr[j][i] = arr[j-1][i];
        arr[j][i] = t;
    } 
}
    
void shiftR(int** arr){
    int i,j,t;
    for(i = 0; i < SIZE; ++i){
        t = arr[i][SIZE-1];
        for(j = SIZE-1; j>0; --j)
            arr[i][j] = arr[i][j-1];
        arr[i][j] = t;        
    }  
}

void shiftG(int** arr){
    shiftD(arr);
    shiftR(arr);    
}

int main(){
    double time;
    
    int** matrix = (int**)malloc(SIZE * sizeof(int*));
    makeMatr(matrix);
    int p,q;
    
    for (p = 0; p<SIZE; ++p){
        for(q = 0; q<SIZE; ++q)
            printf ("%d",matrix[p][q]);
        printf("\n");
    }
    
    clock_t start = clock();
    shiftG(matrix);
    time = clock() - start; 
    printf("\n Time of work: %f \n", time);
    printf("\n \n");
    
    for (p = 0; p<SIZE; ++p){
        for(q = 0; q<SIZE; ++q)
            printf ("%d",matrix[p][q]);
        printf("\n");
    }
   return 0;    
}