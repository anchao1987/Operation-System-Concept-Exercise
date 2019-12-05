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
#include <string.h>
#define MIN_PID  300
#define MAX_PID  5000
int pids[MAX_PID-MIN_PID+1];
int main(int argc, char *argv[])
{

    return 0;
}

int allocate_map()
{
    if (pids != NULL)
    {
        return -1;
    }

    memset(pids, 0, MAX_PID - MIN_PID + 1);
    return 0;
}

int allocate_pid() {
    for (int i = 0; i < MAX_PID-MIN_PID+1; i++) {
        if (pids[i] == 0) {
            return i + MIN_PID;
        }
    }
    return -1;
}

void release_pid(int pid) {
    pids[pid-MIN_PID] = 0;
}