#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define MAX_LINE 80 /* The maximum length command */

int main(void)
{
    char *args[MAX_LINE / 2 + 1]; /* command line arguments */
    int should_run = 1;           /* flag to determine when to exit program */
    char *historyBuf;
    int concurrent = 0;
    int hasHistory = 0;
    while (should_run)
    {
        printf("osh>");
        fflush(stdout);
        size_t len = 0;
        char *buf;
        int read = getline(&buf, &len, stdin);
        if (read == -1 || strlen(buf) == 1)
        {
            free(buf);
            fprintf(stderr, "Read Command Err");
            continue;
        }
        buf[strlen(buf) - 1] = 0;
        if (buf[strlen(buf) - 1] == '&')
        {
            concurrent = 1;
        }

        if (strcmp(buf, "!!") == 0)
        {
            printf("Execute Last Command");

            if (hasHistory == 0)
            {
                fprintf(stderr, "No commands in history.\n");

                free(buf);
                continue;
            }
            memcpy(buf, historyBuf, strlen(historyBuf));
        }
        else if (strcmp(buf, "quit") == 0)
        {
            printf("Quit\n");
            free(buf);
            if (hasHistory == 1)
            {
                free(historyBuf);
            }
            return 0;
        }
        else
        {

            historyBuf = malloc(strlen(buf));
            memcpy(historyBuf, buf, strlen(buf));
        }

        if (buf != NULL)
        {
            hasHistory = 1;
        }

        pid_t pid;
        pid = fork();
        if (pid < 0)
        {
            fprintf(stderr, "Fork Fail");
            continue;
        }
        else if (pid == 0)
        {
            int i = 0;
            char *token;
            token = strtok(buf, " ");
            char *pipeArgs[MAX_LINE / 2 + 1];
            int pipeFlag = 0;
            int pipeFd[2];
            while (token != NULL)
            {
                if (strcmp(token, "<") == 0)
                {
                    //Redirect input
                    token = strtok(NULL, " ");
                    int fd = open(token, O_RDWR, S_IRUSR | S_IWUSR);
                    dup2(fd, STDIN_FILENO);
                    close(fd);
                    i++;
                    break;
                }
                else if (strcmp(token, ">") == 0)
                {
                    // Redirect output
                    token = strtok(NULL, " ");
                    int fd = open(token, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
                    dup2(fd, STDOUT_FILENO);
                    dup2(fd, STDERR_FILENO);
                    close(fd);
                    i++;
                    break;
                }
                else if (strcmp(token, "|") == 0)
                {
                    pipeFlag = 1;
                    token = strtok(NULL, " ");
                    int j = 0;
                    while (token != NULL)
                    {
                        pipeArgs[j] = token;
                        token = strtok(NULL, " ");
                        j++;
                    }
                    pipeArgs[j] = 0;
                    i++;
                    break;
                }
                args[i] = token;
                token = strtok(NULL, " ");

                i++;
            }
            args[i] = NULL;
            if (pipeFlag == 1)
            {
                pipe(pipeFd);
                pid_t cpid = fork();
                if (cpid == 0)
                {
                    close(pipeFd[1]);
                    dup2(pipeFd[0], STDIN_FILENO);
                    close(pipeFd[0]);
                    printf("child command args %s\n", pipeArgs[0]);
                    execvp(pipeArgs[0], &pipeArgs[0]);
                }
                else
                {
                    close(pipeFd[0]);
                    dup2(pipeFd[1], STDOUT_FILENO);
                    close(pipeFd[1]);
                    printf("command args %s %s \n", args[0], args[1]);
;                    execvp(args[0], &args[0]);

                }
            }
            else
            {
                printf("Execute command %s\n", args[0]);
                int ret = execvp(args[0], &args[0]);
                printf("execvp result: %d\n", ret);
            }
        }
        else
        {
            if (concurrent == 0)
            {
                wait(NULL);
            }
            free(buf);
        }
    }
    free(historyBuf);

    return 0;
}
