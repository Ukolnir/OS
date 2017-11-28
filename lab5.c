#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int main (int args, char ** argv)
{
    int fd;
    char resultSum;
    ssize_t ret;
    
    char ch; //!!!!!!
    if (args != 2)
    {
	printf("Incorrect count arguments\n");
	return EXIT_FAILURE;
    }
    
    fd = open (argv[1], O_RDONLY);
    if (fd < 0)
    {
        printf("Cannot open file\n");
	//perror ("Cannot open file\n");
	return EXIT_FAILURE;
    }
    
    ret = read (fd, &ch, sizeof(ch));
    resultSum = (char)ch;
    
    while ((ret = read (fd, &ch, sizeof(ch))) > 0)
    {
        resultSum = resultSum ^ (char)ch;
    }
    
    if (ret < 0)
    {
        printf("Cannot read file\n");
        //perror ("Cannot read file\n");
	return EXIT_FAILURE;
    }
    else
    {
       printf ("Control Sum: %d\n", (int)resultSum);
       close (fd);
       //return 0;
    }
}