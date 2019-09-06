int echo(int tot, char *arr[], int x)
{
    for (int i = x + 1; i < tot; i++)
    {

        printf("%s ", arr[i]);
    }
    printf("\n");
    x = tot;
    return x;
}