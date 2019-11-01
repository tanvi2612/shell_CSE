int bg(int tot, char *arr[], int x)
{
    if (tot != 2)
    {
        perror("Invalid syntax");
        return tot;
    }
    int i;
    for (i = 0; i < 100; i++)
    {
        if (stoparr[i] == 0)
        {
            break;
        }
    }
    if (atoi(arr[1]) > i)
    {
        printf("Invalid job number\n");
        return tot;
    }
    int bg_ind = atoi(arr[1]) - 1;
    int bg_id = procs[bg_ind].pid;
    if (strcmp(procs[bg_ind].pstatus, "Running") == 0)
    {
        return tot;
    }
    if (strcmp(procs[bg_ind].pstatus, "Killed") == 0 || stoparr[bg_ind] == 0)
    {
        printf("Can't resume a killed process!\n");
        return tot;
    }
    kill(bg_id, SIGCONT);
    strcpy(procs[bg_ind].pstatus, "Running");
    return tot;
}

int fg(int tot, char *arr[], int x)
{
    if (tot != 2)
    {
        perror("Invalid syntax");
        return tot;
    }
    int i;
    for (i = 0; i < 100; i++)
    {
        if (stoparr[i] == 0)
        {
            break;
        }
    }
    if (atoi(arr[1]) > i)
    {
        printf("Invalid job number\n");
        return tot;
    }
    int fg_ind = atoi(arr[1]) - 1;
    int fg_id = procs[fg_ind].pid;
    if (strcmp(procs[fg_ind].pstatus, "Killed") == 0 && stoparr[fg_ind] == 0)
    {
        printf("Can't resume a killed process!\n");
        return tot;
    }
    stoparr[fg_ind] = -1;
    strcpy(procs[fg_ind].pstatus, "Killed");
    curproc_pid = fg_id;
    kill(fg_id, SIGSTOP);
    kill(fg_id, SIGCONT);
    int waitt;
    waitpid(-1, &waitt, WUNTRACED);
    if(pid_cnt == fg_ind){
        pid_cnt--;
    }
    return tot;
}