#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <netdb.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>
#include <zconf.h>
#include <fnmatch.h>
#include <signal.h>
//declaration of variables
typedef struct
{
    int pid;
    char pname[100];
    char pstatus[100];
} proces;
proces procs[200];
int sz = 0;
const int arg = 45;
int buf = 1024;
char home_dir[1024];
int childpid[1024] = {0};
int chil_in = 0;
int hist = 0;
char histor[20][1024];
char *name_pname[50];
typedef struct values
{
    bool full;
    pid_t pid;
    char command[100];
} Values;
Values v[100];
pid_t jobarr[100] = {0};
int stoparr[100] = {0};
int shell_pid;
int curproc_pid = -1;
char i_file[100];
char o_file[100];
char a_file[100];
int pid_cnt = 0;
int i_chk;
int o_chk;
int a_chk;
int var_1, var_2, var_3;
#include "prototypes.h"
#include "cd.h"
#include "display.h"
#include "echo.h"
#include "setenvir.h"
#include "ls.h"
#include "pinfo.h"
#include "pwd.h"
#include "history.h"
#include "bg.h"
#include "redirect.h"