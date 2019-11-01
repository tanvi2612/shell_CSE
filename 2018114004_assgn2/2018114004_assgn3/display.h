void display(char *cwd, int sz)
{
    //printf("dir = %s\n", cwd);
    char tilde[strlen(cwd)];
    int k = 1;
    if (sz <= strlen(cwd))
    {
        for (int i = sz; i < strlen(cwd); i++, k++)
        {
            //printf("hel\n");
            tilde[k] = cwd[i];
        }

        tilde[0] = '~';
    }
    else
    {
        for (int i = 0; i < strlen(cwd); i++)
        {
            tilde[i] = cwd[i];
        }
    }

    char hostbuffer[256];
    int hostname = gethostname(hostbuffer, sizeof(hostbuffer));
    if (hostname == -1)
    {
        perror("Error: Hostname");
        exit(-1);
    }
    char sysname[256];
    int loginname = getlogin_r(sysname, sizeof(sysname));
    if (loginname != 0)
    {
        perror("Error: Login");
        exit(1);
    }
    char *print;
    print = strcat(sysname, "@");
    print = strcat(print, hostbuffer);
    print = strcat(print, ":");
    print = strcat(print, tilde);
    print = strcat(print, ">");
    printf("%s", print);
}