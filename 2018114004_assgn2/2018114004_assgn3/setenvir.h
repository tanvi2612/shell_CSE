int setenvir(int tot, char *arr[], int x)
{
    if (tot > 3 || tot < 2)
    {
        perror("Invalid input");
        //exit(-1);
        return tot;
    }
    char *val = tot == 3 ? arr[2] : NULL;
    //char *var = arr[1];
    //strcat(var, val);
    setenv(arr[1], val, 1);
    return tot;
}

int unsetenvir(int tot, char *arr[], int x)
{
    if (tot < 1)
    {
        perror("Invalid input");
        //exit(-1);
        return tot;
    }

    unsetenv(arr[1]);
    return tot;
}

// int filter(const struct dirent *dir)
// {
//     return !fnmatch("[1-9]*", dir->d_name, 0);
// }
// void processdir(const struct dirent *dir)
// {
//      puts(dir->d_name);
// }
int jobs(int tot, char *arr[], int x)
{
    

    for(int n = 0; n < 100; n++)
    {

        if (stoparr[n] ==1)
        {
            char stat_chk;
            char stats[100];
            sprintf(stats, "/proc/%d/stat", procs[n].pid);
            FILE *fd = fopen(stats, "r");
            if(!fd){
                perror("wrong process id");
                return tot;
            }
            fscanf(fd,"%c", &stat_chk);
            if(stat_chk == 'T'){
                strcpy(procs[n].pstatus, "Stopped");
            }
            printf("[%d] %s %s [%d]\n", n + 1, procs[n].pstatus, procs[n].pname, procs[n].pid);
        }
    }
    //free(namelist);
    return tot;
}

int kjob(int tot, char *arr[], int x)
{
    if (tot != 3)
    {
        printf("wrong syntax\n");
        return tot;
    }
    int j_id = atoi(arr[x+1]);
    int counter = -1;
    int imat=j_id-1;

    if(stoparr[j_id-1]==0){
        printf("Invalid jobid\n");
        return tot;
    }
    
    printf("%d %s\n", procs[imat].pid, procs[imat].pname);
    //strcpy(procs[imat].pstatus, "Killed");
    kill(procs[imat].pid, atoi(arr[x+2]));
    if(atoi(arr[x+2]) == 9){
        strcpy(procs[imat].pstatus, "Killed");
        stoparr[imat] = -1;
    }
    return tot;

}

int overkill(int tot, char *arr[], int x){
    if(tot != 1){
        printf("error\n");
        return tot;
    }
    for(int i = 0; i<100; i++){
        if(stoparr[i] == 0){
            break;
        }
        kill(procs[i].pid, SIGKILL);
        strcpy(procs[i].pstatus, "Killed");
        stoparr[i]=-1;

    }
    return tot;
}
