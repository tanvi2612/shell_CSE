int cd(int tot, char *arr[], int x, char home_dir[], int sz)
{
    if (tot > 2)
    {
        perror("too many variables\n");
        x = tot;
        return x;
    }
    if (tot == 1)
    {

        chdir(home_dir);
        
    }
    else if(strcmp(arr[x + 1], "~")==0){
        chdir(home_dir);
        x++;
    }

    else if (strcmp(arr[x + 1], ".") == 0)
    {
        x++;        
    }

    else if (strcmp(arr[x + 1], "/") == 0)
    {
        x++;
        chdir("/");
       
    }
    else if (strcmp(arr[x + 1], "..") == 0)
    {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        //printf("%s\n", cwd);
        int br = 0;
        for (int i = strlen(cwd) - 1; i >= 0; i--)
        {

            if (cwd[i] == '/')
            {
                // printf("%d\n", i);
                br = i;
                break;
            }
        }
        //printf("%d\n", br);
        char newpath[1024];

        if (br >= sz)
        {
            for (int i = 0; i < br; i++)
            {
                newpath[i] = cwd[i];
            }
            newpath[br]='\0';
            //printf("%s\n", newpath);
            int chk = chdir(newpath);

            if (chk != 0)
            {
                perror("error in cd");
            }
        }
        else
        {
            for (int i = 0; i < br; i++)
            {
                newpath[i] = home_dir[i];
            }
            newpath[br] = '\0';
            //printf("%s\n", newpath);
            int chk = chdir(newpath);

            if (chk != 0)
            {
                perror("error in cd2");
            }
        }
        x++;
        
    }
    else
    {

        int chk = chdir(arr[x + 1]);
        if (chk != 0)
        {
            perror("error in cd");
        }
        x++;
    }
    x++;
    return x;
}