
#include "globals.h"
int piper = 0;
int saved_stdout;
//******************************PROGRAM STARTS *********************************
void ctrlc(int sig_num)
{
    if (getpid() != shell_pid)
    {
        return;
    }
    if (curproc_pid != -1)
    {
        kill(curproc_pid, SIGINT);
    }
    int point = 0;
    while (stoparr[point] != 0)
    {
        if (curproc_pid == procs[point].pid)
        {
            strcpy(procs[point].pstatus, "Killed");
            stoparr[point] = -1;
            break;
        }

        point++;
    }
    fflush(stdout);
    signal(SIGINT, ctrlc);
    //printf("\n");
    return;
}
void ctrlz(int sig_num)
{
    if (getpid() != shell_pid)
    {
        return;
    }
    if (curproc_pid != -1)
    {
        kill(curproc_pid, SIGTSTP);
        int point = 0;
        int check_proc = 0;
        while (stoparr[point] != 0)
        {
            if (curproc_pid == procs[point].pid)
            {
                check_proc = 1;
                strcpy(procs[point].pstatus, "Stopped");
                break;
            }

            point++;
        }
        if (check_proc == 0)
        {
            //this process was never present in background
            procs[point].pid = curproc_pid;
            char proc_name[1000];
            sprintf(proc_name, "/proc/%d/cmdline", curproc_pid);
            FILE *fd = fopen(proc_name, "r");
            if (fd != 0)
            {

                int len = fread(proc_name, sizeof(char), 1024, fd);
                if (len > 0)
                {
                    if ('\n' == proc_name[len - 1])
                        proc_name[len - 1] = '\0';
                }
                fclose(fd);
            }
            strcpy(procs[point].pname, proc_name);
            strcpy(procs[point].pstatus, "Stopped");
            stoparr[point] = 1;
            pid_cnt = point;
        }
    }

    signal(SIGTSTP, ctrlz);
    fflush(stdout);
    return;
}
void procend(int sig)
{
    if (getpid() != shell_pid)
    {
        return;
    }
    pid_t cpid;
    int status;
    cpid = waitpid(-1, &status, WNOHANG); //wait till child process is over
    int i = 0;

    while (v[i].pid != cpid && i < 100)
    {
        i++;
    }
    if (v[i].pid == cpid)
    {
        char buffer[buf];
        stoparr[i] = -1;
        if (WIFEXITED(status))
        {
            sprintf(buffer, "\n%s with pid %d exited normally\n", v[i].command, cpid);
            printf("%s\n", buffer);
        }
        if (WIFSIGNALED(status))
        {
            sprintf(buffer, "\n%s with pid %d exited via signal %d\n", v[i].command, cpid, WTERMSIG(status));
            printf("%s\n", buffer);
        }
        strcpy(procs[i].pstatus, "Killed");
    }

    //v[i].full = false;
    //arr[i] = -1;
    signal(SIGCHLD, procend);
    //write(1, buffer, strlen(buffer));
}

void execute(char *arr[], int tot, int in, int out)
{

    for (int x = 0; x < tot;)
    
    {
        
        //printf("tetal %d\n", tot);
        if (arr[x] != NULL && strcmp(arr[x], "&"))
        {

            if (strcmp(arr[x], "cd") == 0)
            {
                x = cd(tot, arr, x, home_dir, sz);
            }
            else if (strcmp(arr[x], "bg") == 0)
            {
                //printf("helo\n");
                x = bg(tot, arr, x);
            }
            else if (strcmp(arr[x], "fg") == 0)
            {
                //printf("helo\n");
                x = fg(tot, arr, x);
            }
            else
            {
                int is_bg = 0;
                if (strcmp(arr[tot - 1], "&") == 0)
                {
                    is_bg = 1;
                    arr[tot - 1] = NULL;
                    //tot -= 1;
                }
                pid_t pid_1, pid_2;
                pid_t cpid = fork();
                curproc_pid = cpid;

                if (cpid == 0)
                {
                    if (is_bg != 1)
                    {
                        redirect(tot, arr);
                        if (i_chk == 1)
                        {
                            dup2(var_1, 0);
                            close(var_1);
                        }

                        if (o_chk == 1)
                        {
                            dup2(var_2, 1);
                            close(var_2);
                        }

                        if (strcmp(arr[x], "pwd") == 0)
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
                        else if (strcmp(arr[x], "setenv") == 0)
                        {
                            //printf("helo\n");
                            x = setenvir(tot, arr, x);
                        }
                        else if (strcmp(arr[x], "unsetenv") == 0)
                        {
                            //printf("helo\n");
                            x = unsetenvir(tot, arr, x);
                        }
                        else if (strcmp(arr[x], "jobs") == 0)
                        {
                            //printf("helo\n");
                            x = jobs(tot, arr, x);
                        }
                        else if (strcmp(arr[x], "kjob") == 0)
                        {
                            //printf("helo\n");
                            x = kjob(tot, arr, x);
                        }

                        else if (strcmp(arr[x], "overkill") == 0)
                        {
                            //printf("helo\n");
                            x = overkill(tot, arr, x);
                        }
                        else {
                            if (execvp(arr[x], arr) < 0)
                        {
                            if(piper == 1){
                                dup2(saved_stdout, 1);
                            }
                            
                            fprintf(stderr, "unknown command\n");
                            return;
                        }
                        x = tot;
                        }
                    }
                    else
                    {

                        if (is_bg == 1)
                        {
                            if (setpgid(0, 0) != 0)
                            {
                                perror("Unknown command");
                                return;
                            }
                        }

                        //printf("new\n");

                        if (execvp(arr[x], arr) < 0)
                        {
                            fprintf(stderr, "unknown command\n");
                            return;
                        }
                        x = tot;
                    }
                }
                else if (cpid < 0)
                {
                    perror("Error in forking");
                    return;
                }
                else if (cpid > 0)
                {
                    if (is_bg)
                    {
                        kill(cpid, SIGTTIN);
                        kill(cpid, SIGCONT);
                        for (int i = 0; i < 100; ++i)
                        {
                            if (!v[i].full)
                            {

                                v[i].full = true;
                                v[i].pid = cpid;
                                strcpy(v[i].command, arr[x]);

                                stoparr[i] = 1;
                                //printf("halo\n");
                                procs[i].pid = cpid;
                                strcpy(procs[i].pname, arr[x]);
                                strcpy(procs[i].pstatus, "Running");
                                pid_cnt = i;

                                i = 100; ///to break out of loop
                            }
                        } //above loop stores the command name and pid
                        //printf("end\n");

                        //signal(SIGCHLD, procend);
                    }
                    else
                    {
                        curproc_pid = cpid;
                        waitpid(cpid, NULL, 0);
                    }
                    is_bg = 0;
                    x = tot;
                }
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
            int pipe_chk = 0, number_pipes[15], pip_ind = 0;
            //printf("chunk = %s\n", brk);
            token = strtok(brk, delim);
            while (token != NULL && argindex < arg - 1)
            {
                //
                if (!strcmp(token, "|"))
                {
                    pipe_chk = 1;
                    number_pipes[pip_ind++] = argindex;
                }
                args[argindex++] = token;

                token = strtok(NULL, delim);
            }

            args[argindex] = '\0';

            if (argindex == 0)
                return; /*  at least 1 arg */

            if (strcmp(args[0], "quit") == 0 || strcmp(args[0], "exit") == 0)
            {
                exit(1);
                exit(1);
            }
            int last_ind = 0;

            int in = 0;
            piper = 0;
            if (pipe_chk == 1)
            {
                piper = 1;
                int var1, var2;

                for (int i = 0; i < pip_ind; i++)
                {
                    
                    int fdp[2];
                    pipe(fdp);
                    pid_t p_pid = fork();
                    saved_stdout = dup(STDOUT_FILENO);
                    if (p_pid == 0)
                    {
                        char *exec_cmd[100];
                        int cmd_p = 0;
                        
                        while (last_ind < number_pipes[i])
                        {
                            exec_cmd[cmd_p++] = args[last_ind++];
                        }
                        
                        dup2(var1, 0);
                        
                        dup2(fdp[1], 1);
                        //printf("halo\n");
                        close(fdp[0]); 

                        //var1 = dup(0);
                        //var2 = dup(1);
                        
                        

                        // redirect(cmd_p, exec_cmd);
                        execute(exec_cmd, cmd_p, in, fdp[1]);
                    }
                    else
                    {
                        //printf("halo2\n");
                        wait(NULL);
                        close(fdp[1]);
                        var1 = fdp[0];
                         last_ind = number_pipes[i] + 1;
                    }
                    // redirect(cmd_p, exec_cmd);
                    // execute(exec_cmd, cmd_p, in, fdp[1]);
                    // close(fdp[1]);

                    in = fdp[0];
                     //last_ind = number_pipes[i] + 1;
                }
                
                int fdp[2];
                pipe(fdp);
                pid_t p_pid = fork();

                if (p_pid == 0)
                {
                    dup2(var1, 0);

                    close(fdp[0]);

                    //var1 = dup(0);
                    //var2 = dup(1);
                    char *exec_cmd[100];
                    int cmd_p = 0;
                    while (last_ind < argindex)
                    {
                        exec_cmd[cmd_p++] = args[last_ind++];
                    }
                    // redirect(cmd_p, exec_cmd);
                    execute(exec_cmd, cmd_p, in, 1);
                }
                else
                {
                    wait(NULL);
                    close(fdp[1]);
                    var1 = fdp[0];
                }
                // redirect(cmd_p, exec_cmd);
                // execute(exec_cmd, cmd_p, in, fdp[1]);
                // close(fdp[1]);

                // in = fdp[0];
                // last_ind = number_pipes[i] + 1;
                // int var3, var4;
                // var3 = dup(0);
                // var4 = dup(1);
                // char *exec_cmd[100];
                // int cmd_p = 0;

                // dup2(var3, 0);
                // dup2(var4, 1);
            }
            else
            {

                //printf("%d %s \n", argindex, args[0]);
                // redirect(argindex, args);
                execute(args, argindex, 0, 1);
            }
        }
        return;
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
    shell_pid = getpid();

    signal(SIGTSTP, ctrlz);
    signal(SIGINT, ctrlc);
    signal(SIGCHLD, procend);
    // signal(SIGTSTP, ctrlz);
    getcwd(home_dir, sizeof(home_dir));
    sz = strlen(home_dir);
    //printf("home = %s\n", home_dir);
    main_loop();
    return EXIT_SUCCESS;
}