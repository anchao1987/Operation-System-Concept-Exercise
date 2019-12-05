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
    int begNum = atoi(argv[1]);
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
        while (begNum != 1) {
            printf("%d,",begNum);
            if (begNum % 2 == 0) {
                begNum = begNum / 2;
            } else {
                begNum = 3 * begNum + 1;
            }
        }
        printf("%d\n",begNum);
    }
    else
    { /* parent process */
        /* parent will wait for the child to complete */

        wait(NULL);
    }
    return 0;
}
