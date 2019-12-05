#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

#define READ_END 0
#define WRITE_END 1
int main(int argc, char *argv[])
{

    struct timeval current;
    int fd[2];
    if (pipe(fd) == -1)
    {
        fprintf(stderr, "Piped Error");
        return 1;
    }
    
    pid_t pid;
    char buffer[20];
    /* fork a child process */
    pid = fork();
    if (pid < 0)
    { /* error occurred */
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0)
    {
        close(fd[READ_END]);
        gettimeofday(&current, NULL);
        sprintf(buffer,"%ld",current.tv_usec);
        printf("Start time : %s\n",buffer);
        write(fd[WRITE_END],buffer,strlen(buffer) + 1);
        close(fd[WRITE_END]);

        /* child process */
        execlp(argv[1], argv[1], NULL);

    }
    else
    { /* parent process */
        /* parent will wait for the child to complete */

        wait(NULL);
        close(fd[WRITE_END]);
        read(fd[READ_END],buffer,50);
        close(fd[READ_END]);
        gettimeofday(&current, NULL);

        printf("Elapsed time: %f\n", (current.tv_usec - atoi(buffer)) / 60000.0);
    }
    return 0;
}
