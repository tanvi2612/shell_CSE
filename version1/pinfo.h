int pinfo(int tot, char *arr[], int x)
{
    char pid[105];
    if (tot == 1)
    {
        sprintf(pid, "%d", getpid());
    }
    else if (tot == 2)
    {
        strcpy(pid, arr[x + 1]);
        x++;
    }
    else
    {
        perror("Too many arguments");
        return tot;
    }

    //printf("pid -- %d\n", getpid());

    char buffer[1024] = "";

    int fd = open("/proc/self/stat", O_RDONLY);
    read(fd, buffer, sizeof(buffer));
    char *list[800];
    char *temp = strtok(buffer, " \r\t\n");
    int j = 0;
    while (temp != NULL)
    {
        list[j++] = temp;
        temp = strtok(NULL, " \r\t\n");
    }
    printf("pid -- %s\n", list[0]);

    printf("Process Status -- %s\n", list[2]); //process status
    printf("memory -- %s\n", list[23]);        //memory amount

    char *execprin = realpath("/proc/self/exe", NULL);

    char tilda[strlen(execprin)];
    int k = 1;
    if (sz <= strlen(execprin))
    {
        for (int i = sz; i < strlen(execprin); i++, k++)
        {
            //printf("hel\n");
            tilda[k] = execprin[i];
        }

        tilda[0] = '~';
    }
    else
    {
        for (int i = 0; i < strlen(execprin); i++)
        {
            tilda[i] = execprin[i];
        }
    }

    printf("Executable -- %s\n", tilda);
    close(fd);
    x++;
    return x;
}