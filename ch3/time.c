#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{

    struct timeval current;
    const int SIZE = 1024;
    const char *name = "time";
    int fd;
    long *ptr;
    fd = shm_open(name,O_CREAT | O_RDWR, 0666);
        printf("File description %d\n", fd);

        ftruncate(fd,SIZE);
        ptr = (long *)mmap(NULL,SIZE,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    pid_t pid;
    /* fork a child process */
    pid = fork();
    if (pid < 0)
    { /* error occurred */
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0)
    {
        
        gettimeofday(&current,NULL);
        *ptr = current.tv_usec;
         /* child process */
         execlp(argv[1],argv[1], NULL);
    }
    else
    {   /* parent process */
        /* parent will wait for the child to complete */

        wait(NULL);
        gettimeofday(&current,NULL);

        printf("Elapsed time: %f\n",(current.tv_usec - *ptr) / 60000.0 );
        shm_unlink(name);
    }
    return 0;
}
