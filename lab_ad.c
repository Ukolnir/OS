#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char * argv[]){
    int fin, len, ei, bool;
    char *sr,  *c;
    
    struct stat sb;

    if (argc != 2){
        printf ("Incorrect count arguments\n");
        return EXIT_FAILURE;
    }
    if ((fin = open(argv[1], O_RDONLY)) < 0){
	perror ("Невозможно окрыть файл на чтение. \n");
       	return EXIT_FAILURE;
    }

    if (fstat(fin, &sb) < 0){
	perror ("Error! - fstat\n");
       	return EXIT_FAILURE;
    }

    if ((sr = (char*)mmap(0,sb.st_size, PROT_READ, MAP_SHARED, fin,0)) 
==MAP_FAILED){
        perror ("Error! - mmap (with readfile)\n");
        return EXIT_FAILURE;
	}

    close(fin);
    
    c = sr;
    len = sb.st_size;
    bool = 1;
    
    for(int i = 0; i < len/2; ++i){
        if(*(c+i) != *(c+len-1-i)){
            printf("False \n");
            bool = 0;
            break;
        }        
    }
    if (bool == 1)
        printf("True \n");

    munmap((void *)sr, sb.st_size);
    return 0;
}