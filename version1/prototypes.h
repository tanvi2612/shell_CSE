void **dirnames(int a, int p, struct dirent **dir);
void dirperms(char arr[]);
int ls(int tot, char *arr[], int x);
int echo(int tot, char *arr[], int x);
int pwd(int tot, char *arr[], int x);
int cd(int tot, char *arr[], int x, char home_dir[], int sz);
int pinfo(int tot, char *arr[], int x);
void display(char *cwd, int sz);

void procend(int sig);
void execute(char *arr[], int tot);
void readstr();
void main_loop();
