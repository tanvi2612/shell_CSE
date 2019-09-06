#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <netdb.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>
#include <zconf.h>

//declaration of variables
int sz = 0;
const int arg = 45;
int buf = 1024;
char home_dir[1024];
int childpid[1024] = {0};
int chil_in = 0;
int hist = 0;
char histor[20][1024];
typedef struct values
{
    bool full;
    pid_t pid;
    char command[100];
} Values;
Values v[100];

#include "prototypes.h"
#include "cd.h"
#include "display.h"
#include "echo.h"

#include "ls.h"
#include "pinfo.h"
#include "pwd.h"
#include "history.h"

//******************************PROGRAM STARTS *********************************

void procend(int sig)
{
    pid_t cpid;
    int status;
    cpid = waitpid(-1, &status, WNOHANG); //wait till child process is over
    int i = 0;
    while (v[i].pid != cpid && i < 100)
    {
        i++;
    }

    char buffer[buf];
    sprintf(buffer, "\n%s with pid %d exited normally\n", v[i].command, cpid);
    printf("%s\n", buffer);
    //write(1, buffer, strlen(buffer));
}

void execute(char *arr[], int tot)
{
    for (int x = 0; x < tot;)
    {
        //printf("tetal %d\n", tot);
        if (strcmp(arr[x], "&"))
        {

            if (strcmp(arr[x], "cd") == 0)
            {
               x = cd(tot, arr, x, home_dir, sz);
            }
            else if (strcmp(arr[x], "pwd") == 0)
            {
                x = pwd(tot, arr, x);
            }
            else if (strcmp(arr[x], "echo") == 0)
            {
                x = echo(tot, arr, x);
            }

            else if (strcmp(arr[x], "ls") == 0)
            {
                x = ls(tot, arr, x);
            }
            else if (strcmp(arr[x], "pinfo") == 0)
            {
                x = pinfo(tot, arr, x);
            }
            else if (strcmp(arr[x], "history") == 0)
            {
                x = history(tot, arr, x, hist);
            }

            else
            {
                //printf("new\n");
                int is_bg = 0;
                pid_t cpid;
                if (strcmp(arr[tot - 1], "&") == 0)
                {
                    is_bg = 1;
                    arr[tot - 1] = NULL;
                    //tot -= 1;
                }

                int i;
                cpid = fork();
                if (cpid == 0)
                {

                    if (execvp(arr[x], arr) < 0)
                    {
                        fprintf(stderr, "unknown command\n");
                        return;
                    }
                }
                else if (cpid < 0)
                {
                    perror("Error in forking");
                    return;
                }
                else
                {
                    if (is_bg)
                    {
                        for (int i = 0; i < 100; ++i)
                        {
                            if (!v[i].full)
                            {
                                v[i].full = true;
                                v[i].pid = cpid;
                                strcpy(v[i].command, arr[x]);
                                i = 100; ///to break out of loop
                            }
                        } //above loop stores the command name and pid
                        signal(SIGCHLD, procend);
                    }
                    else
                    {
                        waitpid(cpid, NULL, 0);
                    }
                    is_bg = 0;
                }
                x = tot;
            }
        }
    }
}

void readstr()
{

    char str[1024] = "";

    char *args[arg], delim[] = " \t\n";
    char *semcol[1024];
    for (int i = 0; i < arg; i++)
    {
        args[i] = NULL;
    }
    if (fgets(str, buf, stdin) == NULL)
    {
        perror("Error404");
    }
    if (strlen(str) == 0 || *str == '\n')
    {
        return;
    }
    else
    {
        char *token;
        char *cmds;
        int l = 0;
        cmds = strtok(str, ";");
        while (cmds != NULL)
        {
            semcol[l++] = cmds;
            cmds = strtok(NULL, ";");
        }
        semcol[l] = '\0';
        for (int i = 0; i < l; i++)
        {
            int argindex = 0;
            if (strlen(semcol[i]) == 0)
            {
                continue;
            }
            strcpy(histor[hist % 20], semcol[i]);
            hist++;
            char brk[strlen(semcol[i])];
            strcpy(brk, semcol[i]);
            //printf("chunk = %s\n", brk);
            token = strtok(brk, delim);
            while (token != NULL && argindex < arg - 1)
            {
                //
                args[argindex++] = token;

                token = strtok(NULL, delim);
            }
            args[argindex] = '\0';

            if (argindex == 0)
                return; /*  at least 1 arg */

            if (strcmp(args[0], "quit") == 0 || strcmp(args[0], "exit") == 0)
                exit(1);

            execute(args, argindex);
        }
    }
}

void main_loop()
{
    while (1)
    {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));

        display(cwd, sz);
        readstr();
    }
}
int main(void)
{
    //char cwd[1024];
    getcwd(home_dir, sizeof(home_dir));
    sz = strlen(home_dir);
    //printf("home = %s\n", home_dir);
    main_loop();
    return EXIT_SUCCESS;
}