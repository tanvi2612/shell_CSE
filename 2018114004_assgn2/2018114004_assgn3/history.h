int history(int tot,char *arr[], int x, int hist){
    if(tot > 2){
        perror("too many arguments");
        return tot;
    }
    int limit=10;
    if (tot == 2){
        limit=atoi(arr[1]);
    }
    int i; //takes value given by the user as limit
    if(limit>hist){
        i = hist;
    }
    else {
        i = limit;
    }
    while(i>0)
    {

        printf("%s",histor[(hist-i)%20]);
        i--;
    }
    
}
