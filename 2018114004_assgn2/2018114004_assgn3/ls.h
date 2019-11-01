char *names[1024];
void **dirnames(int a, int p, struct dirent **dir)
{

    if (a == 0)
    {

        for (int i = 2; i < p; i++)
        {
            //printf("helo\n");
            names[i - 2] = dir[i]->d_name;
        }
    }
    else
    {

        for (int i = 0; i < p; i++)
        {
            names[i] = dir[i]->d_name;
        }
    }
}
void dirperms(char arr[])
{
    struct stat fin;
    stat(arr, &fin);
    printf((S_ISDIR(fin.st_mode)) ? "d" : "");
    printf((S_ISREG(fin.st_mode)) ? "-" : "");
    printf((S_ISLNK(fin.st_mode)) ? "l" : "");

    (fin.st_mode & S_IRUSR) ? printf("r") : printf("-");
    (fin.st_mode & S_IWUSR) ? printf("w") : printf("-");
    (fin.st_mode & S_IXUSR) ? printf("x") : printf("-");

    (fin.st_mode & S_IRGRP) ? printf("r") : printf("-");
    (fin.st_mode & S_IWGRP) ? printf("w") : printf("-");
    (fin.st_mode & S_IXGRP) ? printf("x") : printf("-");

    (fin.st_mode & S_IROTH) ? printf("r") : printf("-");
    (fin.st_mode & S_IWOTH) ? printf("w") : printf("-");
    (fin.st_mode & S_IXOTH) ? printf("x") : printf("-");
}

int ls(int tot, char *arr[], int x)
{
    if (tot == 1)
    {

        char dirname[1024];
        getcwd(dirname, sizeof(dirname));
        int p;
        struct dirent **d;
        //dirname = arr[1]
        p = scandir(dirname, &d, NULL, alphasort);
        //printf("p is %d\n", p);
        if (p == -1)
        {
            perror("Cannot find directory");
            return ++x;
        }
        int k = 0;
        dirnames(0, p, d);
        while (k < p - 2)
        {
            //printf("hello\n");

            printf("%s\n", names[k]);

            free(d[k]);
            k++;
        }
        free(d);
    }
    else if (tot == 2 && strcmp(arr[x + 1], "~") == 0)
    {
        char *dirname = home_dir;

        int p;
        struct dirent **d;
        //dirname = arr[1]
        p = scandir(dirname, &d, NULL, alphasort);

        if (p == -1)
        {
            perror("Cannot find directory");
            return ++x;
        }
        int k = 0;
        dirnames(0, p, d);
        while (k < p - 2)
        {

            printf("%s\n", names[k]);

            free(d[k]);
            k++;
        }
        free(d);
        x++;
    }
    else if (tot == 2 && strcmp(arr[x + 1], "-a") == 0)
    {
        char dirname[1024];
        getcwd(dirname, sizeof(dirname));
        int p;
        struct dirent **d;
        //dirname = arr[1]
        p = scandir(dirname, &d, NULL, alphasort);

        if (p == -1)
        {
            perror("Cannot find directory");
            return ++x;
        }
        int k = 0;
        dirnames(1, p, d);
        while (k < p)
        {

            printf("%s\n", names[k]);

            free(d[k]);
            k++;
        }
        free(d);
        x++;
    }
    else if (tot == 2 && strcmp(arr[x + 1], "-l") == 0)
    {
        char dirname[1024];
        getcwd(dirname, sizeof(dirname));
        int p;
        struct dirent **d;
        //dirname = arr[1]
        p = scandir(dirname, &d, NULL, alphasort);

        if (p == -1)
        {
            perror("Cannot find directory");
            return ++x;
        }
        int k = 0;
        struct stat prin;
        dirnames(0, p, d);
        while (k < p - 2)
        {

            stat(names[k], &prin);

            dirperms(names[k]);

            printf("\t%ld", prin.st_nlink);

            printf("\t%s", getpwuid(prin.st_uid)->pw_name);
            printf("\t%s", getgrgid(prin.st_gid)->gr_name);
            printf("\t%ld\t", prin.st_size);
            char delim[] = " \t\r\n\v\f";
            char *time_mod = ctime(&prin.st_mtime);
            int i = 4;
            while (i < 16)
            {
                printf("%c", time_mod[i]);
                i++;
            }

            printf("\t%s", names[k]);
            free(d[k]);
            k++;
            printf("\n");
        }
        free(d);
        x++;
    }
    else if (tot == 2 && (strcmp(arr[x + 1], "-la") == 0 || strcmp(arr[x + 1], "-al") == 0))
    {
        char dirname[1024];
        getcwd(dirname, sizeof(dirname));
        int p;
        struct dirent **d;
        //dirname = arr[1]
        p = scandir(dirname, &d, NULL, alphasort);

        if (p == -1)
        {
            perror("Cannot find directory");
            return ++x;
        }
        int k = 0;
        struct stat prin;
        dirnames(1, p, d);
        while (k < p)
        {

            stat(names[k], &prin);

            dirperms(names[k]);

            printf("\t%ld", prin.st_nlink);

            printf("\t%s", getpwuid(prin.st_uid)->pw_name);
            printf("\t%s", getgrgid(prin.st_gid)->gr_name);
            printf("\t%ld\t", prin.st_size);
            char delim[] = " \t\r\n\v\f";
            char *time_mod = ctime(&prin.st_mtime);
            int i = 4;
            while (i < 16)
            {
                printf("%c", time_mod[i]);
                i++;
            }

            printf("\t%s", names[k]);
            free(d[k]);
            k++;
            printf("\n");
        }
        free(d);
        x++;
    }
    else if (tot == 3 && strcmp(arr[x + 1], "-l") == 0 && strcmp(arr[x + 2], "-a") == 0)
    {
        char dirname[1024];
        getcwd(dirname, sizeof(dirname));
        int p;
        struct dirent **d;
        //dirname = arr[1]
        p = scandir(dirname, &d, NULL, alphasort);

        if (p == -1)
        {
            perror("Cannot find directory");
            return ++x;
        }
        int k = 0;
        struct stat prin;
        dirnames(1, p, d);
        while (k < p)
        {

            stat(names[k], &prin);

            dirperms(names[k]);

            printf("\t%ld", prin.st_nlink);

            printf("\t%s", getpwuid(prin.st_uid)->pw_name);
            printf("\t%s", getgrgid(prin.st_gid)->gr_name);
            printf("\t%ld\t", prin.st_size);
            char delim[] = " \t\r\n\v\f";
            char *time_mod = ctime(&prin.st_mtime);
            int i = 4;
            while (i < 16)
            {
                printf("%c", time_mod[i]);
                i++;
            }

            printf("\t%s", names[k]);
            free(d[k]);
            k++;
            printf("\n");
        }
        free(d);
        x += 2;
    }
    else if (tot == 3 && strcmp(arr[x + 1], "-a") == 0 && strcmp(arr[x + 2], "-l") == 0)
    {
        char dirname[1024];
        getcwd(dirname, sizeof(dirname));
        int p;
        struct dirent **d;
        //dirname = arr[1]
        p = scandir(dirname, &d, NULL, alphasort);

        if (p == -1)
        {
            perror("Cannot find directory");
            return ++x;
        }
        int k = 0;
        struct stat prin;
        dirnames(1, p, d);
        while (k < p)
        {

            stat(names[k], &prin);

            dirperms(names[k]);

            printf("\t%ld", prin.st_nlink);

            printf("\t%s", getpwuid(prin.st_uid)->pw_name);
            printf("\t%s", getgrgid(prin.st_gid)->gr_name);
            printf("\t%ld\t", prin.st_size);
            char delim[] = " \t\r\n\v\f";
            char *time_mod = ctime(&prin.st_mtime);
            int i = 4;
            while (i < 16)
            {
                printf("%c", time_mod[i]);
                i++;
            }

            printf("\t%s", names[k]);
            free(d[k]);
            k++;
            printf("\n");
        }
        free(d);
        x += 2;
    }
    else if(tot > 2){
        perror("Too many arguments");
        return tot;
    }
    else
    {

        char *dirname = arr[x + 1];

        int p;
        struct dirent **d;
        //dirname = arr[1]
        p = scandir(dirname, &d, NULL, alphasort);

        if (p == -1)
        {
            perror("Cannot find directory");
            return ++x;
        }
        int k = 0;
        dirnames(0, p, d);
        while (k < p - 2)
        {

            printf("%s\n", names[k]);

            free(d[k]);
            k++;
        }
        free(d);
        x++;
    }
    x++;
    return x;
}