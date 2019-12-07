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

    const int SIZE = 4086;
    const char *name = "collatz";
    int begNum = atoi(argv[1]);
    int fd;
    char *ptr;
    fd = shm_open(name, O_CREAT | O_RDWR, 0666);

    ftruncate(fd, SIZE);
    ptr = (char *)mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
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
        while (begNum != 1)
        {
            int size = sprintf(ptr, "%d,", begNum);
            ptr += size;
            if (begNum % 2 == 0)
            {
                begNum = begNum / 2;
            }
            else
            {
                begNum = begNum * 3 + 1;
            }
        }
        int size = sprintf(ptr, "%d", begNum);
        ptr += size;
    }
    else
    { /* parent process */
        /* parent will wait for the child to complete */

        wait(NULL);

        printf("%s\n", ptr);
        shm_unlink(name);
    }
    return 0;
}
