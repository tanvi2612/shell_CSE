int pwd(int tot, char *arr[], int x)
{
    if (tot > 1)
    {
        perror("Too many variables");
        return tot;
    }
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("%s\n", cwd);
    x++;
    return x;
}