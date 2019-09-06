#include <stdio.h>
#include <zconf.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <wait.h>
#include <fcntl.h>


// Defining colors for usage later
#define KRED  "\x1B[31m"
#define KBLU  "\x1B[34m"
//defining a default buffer for inputs
#define BUFSIZE 10000
//defining a maximum number of arguments
#define ARGMAX 10
#define DEF "\x1B[0m" //colour reset

//a greeting string which is constant
const char greeting[BUFSIZE]= "\t\t\t\t          __\n"
                             "\t\t  ___  \t             ____/ /_    _____  __      __   \n"
                             "\t\t / __\\    ____      / __/  _ \\  /      / /     / /          \n"
                             "\t\t/ /___   |____|\t   (_  )  / // /===== / /___  / /___ \n"
                             "\t\t\\____/\t          /___//_/ // /_____ /_____/ /_____/ \t\t\t\t\t  \t\t                                                         \n....by Sumanth Balaji!\n\n";


//an exiting line
const char ending[BUFSIZE]="Exiting the C shell, Thank You.";

typedef struct values{
    bool full;
    pid_t pid;
    char command[100];
}Values;
Values v[100];

bool exiter(size_t argc,char **argv,bool background);
bool cd(size_t argc,char **argv,bool background);
bool clear(size_t argc,char **argv,bool background);
bool pinfo(size_t argc,char **argv,bool background);
bool pwd(size_t argc,char **argv,bool background);
bool echo(size_t argc,char **argv,bool background);
bool ls(size_t argc,char **argv,bool background);
bool historyp(size_t argc,char **argv,bool background);

char history[20][BUFSIZE];
int hist_counter=0;
const char * builtin_C[]={"exit","cd","clear","pwd","echo","ls","history","pinfo"};

const bool (*builtin_F[])(const size_t ,char **,const bool)={
        (const bool (*)(const size_t, char **, const bool)) &exiter,
        (const bool (*)(const size_t, char **, const bool)) &cd,
        (const bool (*)(const size_t, char **, const bool)) &clear,
        (const bool (*)(const size_t, char **, const bool)) &pwd,
        (const bool (*)(const size_t, char **, const bool)) &echo,
        (const bool (*)(const size_t, char **, const bool)) &ls,
        (const bool (*)(const size_t, char **, const bool)) &historyp,
        (const bool (*)(const size_t, char **, const bool)) &pinfo};
//setup for function call back mechanism

void cwd(char* cwdstr, int command);
//helper function
void UsernHost_names(char usernhoststring[]);
//helper function
void adjust_start(char temp[10000]);
//helper function

void bubbleout(char *string);
//helper function

void prompt(char cwdstr[],char usernhoststring[]);

size_t input(size_t argc, char **argv, bool *background);
//input accepting and processing

bool execution(size_t argc, char **argv, bool background);
//execution module that directs the action

void add_history(char *command);

void input1(char **pString);
size_t input2(size_t argc, char **argv, bool *background, char *command);

int main(){//semincolon separated commands in the command line due to assignment requirement
    printf("%s%s",KBLU,greeting);

    char cwdstr[BUFSIZE],usernhoststring[2*BUFSIZE];
    bool exit=false;

    while(!exit){
        prompt(cwdstr,usernhoststring);
        char **commands=malloc(sizeof(char)*BUFSIZE);
        input1(commands); //taking all the commands given
        for (int i = 0; commands[i] && !exit; ++i) {
            size_t argc=0;
            char **argv = malloc(sizeof(char)*ARGMAX);
            bool background=false;
            argc=input2(argc,argv,&background,commands[i]);
            exit=execution(argc,argv,background);

            //exit takes value true in case of errors and whatnot
        }
        free(commands[0]);
        free(commands); //frees up command
    }

    printf("%s%s",KBLU,ending);
    return 0;
}


void input1(char **pString) {
    fflush(stdout); // clear out the stdout buffer just in case

    char *command= malloc(sizeof(char)*BUFSIZE);
    size_t buf = 0;
    getline(&command, &buf, stdin);

    int i=0;
    char * arg=strtok(command,";");

    while(arg != NULL)
    {
        pString[i]=arg;
        ++i;
        arg=strtok(NULL,";"); //because in repeat strtoks, null must be passed
    }
    pString[i]=NULL;
}

size_t input2(size_t argc, char **argv, bool *background, char *command) {
    char checker[1000];
    strcpy(checker,command);

    if (!strcmp(command,"\n"))
        add_history(command);
    argc = 0;
    *background = false;
    char * arg=strtok(command," \t\n");

    while(arg != NULL && argc < ARGMAX-1)
    {
        argv[argc]=arg;
        if(strcmp(argv[argc],"&")==0)
        {
            --argc; //so that the & symbol is overwritten with next string
            *background = true; //run in inBackground
        }

        ++argc;
        arg=strtok(NULL," \t\n"); //because in repeat strtoks, null must be passed
    }
    argv[argc]=NULL;

    return argc;
    //the data pointed to by command is pointed to by argv now, hence it must be deleted later
}



void child_end(int sig) {
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG); //wait till child process is over
    int i=0;
    while (v[i].pid!=pid&&i<100){
        ++i;
    }//finds the position that has the name of the command
    char buffer[BUFSIZE];
    sprintf(buffer,"\n%s with pid %d exited normally\n",v[i].command,pid);
    write(1, buffer, strlen(buffer));

}

bool execution(size_t argc, char **argv, bool background) {

    if (argv[0]==NULL){
        return false;
    }//base case

    bool redirect_in,redirect_out;

    //loop through to check for redirects and piping, i.e > < >> and |
    //This is to be handled in later stage
    for (int i = 0; argv[i] ; ++i) {
        if (!strcmp(argv[i],">")){

        } //redirect out
        else if (!strcmp(argv[i],">>")){

        } //redirect out
        else if (!strcmp(argv[i],"|")){

        } //redirect in
        else if (!strcmp(argv[i],"<")){

        } //redirect in
    }

    for (int j = 0;builtin_C[j]; ++j) {

        if (!strcmp(argv[0],builtin_C[j])){

            return (builtin_F[j])(argc,argv,background);

        }
    }
    //if it reaches here, the command is not a builtin command
    pid_t pid;
    if ((pid=fork())<0){ perror("Error in forking\n");}
    else if(pid==0){///in child
        if (execvp(argv[0],argv)<0){
            printf("Error: execution of command failed\n");
            exit(1);
        }
    } else{
        if (!background){ ///if it is not a background process, its run and finished
            waitpid(pid,NULL,0);
        } else{
            for (int i = 0; i < 100 ; ++i) {
                if(!v[i].full){
                    v[i].full=true;
                    v[i].pid=pid;
                    strcpy(v[i].command,argv[0]);
                    i=100; ///to break out of loop
                }
            }//above loop stores the command name and pid
          signal(SIGCHLD,child_end);
        }
    }
    return false; //default return
}



void UsernHost_names(char usernhoststring[]){
    char *lgn,host[BUFSIZE];
    lgn=(char *)malloc(100* sizeof(char));
    if ((getlogin_r(lgn,100)) || (gethostname(host, sizeof(host))) == -1 ){
        perror("Error in getting user and host name");
    } else{
        strcpy(usernhoststring,lgn);
        strcat(usernhoststring, "@");
        strcat(usernhoststring,host);
    }
    free(lgn);
}

void cwd(char* cwdstr,int command)
{
    char temp[BUFSIZE];

    if(getcwd(temp, sizeof(temp)) != NULL)
    {
        adjust_start(temp);

        strcpy(cwdstr,temp);
        if(command==1)  // check if pwd is to be printed
        {
            printf("\n%s%s\n",KRED,cwdstr);
        }
    }
    else perror("+--- Error in getcwd() : ");
}

void prompt(char cwdstr[],char usernhoststring[]) {
    cwd(cwdstr,0);
    UsernHost_names(usernhoststring);

    printf("%s<%s:%s>",KRED,usernhoststring,cwdstr);

}

void adjust_start(char temp[]) {
    char new_home[10000];
    strcpy(new_home,"/home/sumanth/OS/2018114002_CShell");
    int i = 0;
    for (; new_home[i] && new_home[i]==temp[i] ; ++i) {}
    if (i<=strlen(temp))
    {
        char replace[10000];
        strcpy(replace,"~");
        int z=1;
        while (temp[i]){
            replace[z]=temp[i];
            ++z;
            ++i;
        }
        replace[z]='\0';
        strcpy(temp,replace);
    }    //else the path is outside of shell, hence no edits

}

void bubbleout(char string[]) {
    int j = 0;
    for (; string[j+1] ; ++j) {
        string[j]^=string[j+1];
        string[j+1]^=string[j];
        string[j]^=string[j+1];
        printf("%s\n",string);
    }
    string[j]^=string[j+1];
    string[j+1]^=string[j];
    string[j]^=string[j+1];


    //this code bubbles out the first character
}

bool cd(const size_t argc,char **argv,const bool background){
    //returns true in case of failure after warning
    if(argc!=2){
        printf("%sError, command is incorrect",KRED);
    }
    if(argv[1][0]=='~'){
        char new_home[10000];
        strcpy(new_home,"/home/sumanth/OS/2018114002_CShell");
        bubbleout(argv[1]);
        strcat(new_home,argv[1]);
        strcpy(argv[1],new_home);
    }//changing the meaning of ~

    if(argv[1][0]=='-'){// To be handled later
        ;
    }
    if (chdir(argv[1])){//if unsuccessful, goes into the condition
        printf("%sError, directory %s is not found",KRED,argv[1]);
    }
    pwd(argc,argv,background);
    return false;
}

bool exiter(const size_t argc,char **argv,const bool background) {

    return true;
}

bool clear(const size_t argc,char **argv,const bool background){
    printf ("\033c");
    return false;
}

bool pwd(const size_t argc,char **argv,const bool background){
    char *temp = NULL;
    temp=malloc(sizeof(char)*BUFSIZE);
    cwd(temp,1);

    free(temp);
    return false;
}

bool echo(const size_t argc,char **argv,const bool background){
    //as of now prints whatever it is told
//    if (!background){
        for (int i = 1; argv[i]; ++i) {
            printf("%s%s",KRED,argv[i]);
        }
        printf("\n");

    return false;

}

bool ls(const size_t argc,char **argv,const bool background){

    int i=0;
    char timer[14];
    struct dirent **listr;
    struct stat details;

    ///argv[0] is ls, for 1 to argc, check if first letter is a .
    char filepath[BUFSIZE];
    strcpy(filepath,".");///default

    bool given=false;

    int state=0;// 0 if just ls, 1 if -a , 2 if -l, 3 if -a and -l

    for (int j = 0; !given&&argv[j] ; ++j) {

        if(argv[j][0]=='.'||argv[j][0]=='~'){

            strcpy(filepath,argv[j]);
            given=true;
        }
        else if (strcmp(argv[j],"-a")==0){
            state+=1;
        }
        else if (strcmp(argv[j],"-l")==0){
            state+=2;
        }
        else if (strcmp(argv[j],"-al")==0||strcmp(argv[j],"-la")==0){
            state+=3;
        }
    }

    char current[BUFSIZE];
    ///finding the current directory

    if(getcwd(current, sizeof(current)) == NULL)
         perror("+--- Error in getcwd() : ");

    ///changing into the directory before doing ls. changing out of it is done in the end

    if(filepath[0]=='~'){
        char new_home[10000];
        strcpy(new_home,"/home/sumanth/OS/2018114002_CShell");
        bubbleout(filepath);
        strcat(new_home,filepath);
        strcpy(filepath,new_home);
    }//changing the meaning of ~

    if (chdir(filepath)){//if unsuccessful, goes into the condition
        printf("%sError, directory %s is not found",KRED,filepath);
    }

    ///control now exists in the given directory
    int listn = scandir(".",&listr,NULL,alphasort);//takes everything in the input directory

    if(listn > 0)//listn contains the number of entries in the specified directory
    {
        for ( i = 0; i < listn; i++)
        {
            if(state==0||state==1){ //nothing || -a
                if(( (listr[i]->d_name)[0]=='.' || !strcmp(listr[i]->d_name,".."))&&state==0)
                {
                    continue;
                }//skips hidden files

                if(listr[i]->d_type == DT_DIR)    // a directory
                {
                    printf("%s/%s", listr[i]->d_name, "\n");
                }
                else                              // unknown file types
                {
                    printf("%s%s", listr[i]->d_name, "\n");
                }

                //this functions as a just ls or ls with -a
            }
            else if(state==2||state==3){   //l|| -la

                if(( (listr[i]->d_name)[0]=='.' || !strcmp(listr[i]->d_name,".."))&&state==2)
                {
                    continue;
                }//skips hidden files


                if(stat(listr[i]->d_name,&details)==0)
                {
                    // owner permissions - group permissions - other permissions
                    printf("%s%1s",DEF,(S_ISDIR(details.st_mode)) ? "d" : "-");
                    printf("%s%1s",DEF,(details.st_mode & S_IRUSR) ? "r" : "-");
                    printf("%s%1s",DEF,(details.st_mode & S_IWUSR) ? "w" : "-");
                    printf("%s%1s",DEF,(details.st_mode & S_IXUSR) ? "x" : "-");
                    printf("%s%1s",DEF,(details.st_mode & S_IRGRP) ? "r" : "-");
                    printf("%s%1s",DEF,(details.st_mode & S_IWGRP) ? "w" : "-");
                    printf("%s%1s",DEF,(details.st_mode & S_IXGRP) ? "x" : "-");
                    printf("%s%1s",DEF,(details.st_mode & S_IROTH) ? "r" : "-");
                    printf("%s%1s",DEF,(details.st_mode & S_IWOTH) ? "w" : "-");
                    printf("%s%1s",DEF,(details.st_mode & S_IXOTH) ? "x" : "-");
                    // links associated - owner name - group name
                    printf("%2ld ",(unsigned long)(details.st_nlink));
                    printf("%s ",(getpwuid(details.st_uid))->pw_name);
                    printf("%s ",(getgrgid(details.st_gid))->gr_name);
                    // file size (bytes) - time modified - name
                    printf("%5lld ",(unsigned long long)details.st_size);
                    strftime (timer,14,"%h %d %H:%M",localtime(&details.st_mtime));
                    printf("%s ",timer);
                    if(listr[i]->d_type == DT_DIR)    // a directory
                    {
                        printf("%s/%s", listr[i]->d_name, "\n");
                    }
                    else                              // unknown file types
                    {
                        printf("%s%s", listr[i]->d_name, "\n");
                    }

                }
            }
        }
    }
    else
    {
        printf("+--- Empty directory\n" );
    }

    ///now shifting back into original directory
    if (chdir(current)){//if unsuccessful, goes into the condition
        printf("%sError, directory %s is not found",KRED,current);
    }


    for (int i = 0; i < listn; i++)
    {
        free(listr[i]);
    }

    free(listr);

    return false;

}

bool historyp(const size_t argc,char **argv,const bool background){
    int limit=10;
    if (argc == 2){
        limit=atoi(argv[1]);
    } //takes value given by the user as limit
    for (int i=(limit>hist_counter)?hist_counter:limit;i>0;--i) {
        printf("%s%s\n",KRED,history[(hist_counter-i)%20]);
    }
    return false;
}

void add_history(char *command) {
    int pos=hist_counter%20;
    strcpy(history[pos],command);
    ++hist_counter;
}

bool pinfo(const size_t argc,char **argv,const bool background) {
    char path[200];
    strcpy(path, "/proc/");

    char pid[100];
    if(argc==1){
        sprintf(pid, "%d", getpid());
    }else{
        strcpy(pid,argv[1]);
    }

    strcat(path, pid);

    char exepath[200];
    strcpy(exepath,path);
    strcat(exepath,"/exe");

    strcat(path, "/stat");
    int fd = open(path, O_RDONLY);
    char readstat[800];
    read(fd, readstat, 100);

    char* list[800];
    char * temp = strtok(readstat, " \r\t\n");
    int j=0;
    for(;temp != NULL;++j)
    {
        list[j] = temp;
        temp = strtok(NULL, " \r\t\n");
    }
    list[j] = NULL;

    char execlink[200] = "";
    readlink(exepath, execlink, sizeof(execlink));
    adjust_start(execlink);
    printf("\npid -- %s\n",  list[0]);//pid
    printf("Process Status -- %s\n",  list[2]);//process status
    printf("memory -- %s\n", list[23]); //memory amount
    printf("Executable Path -- %s\n",execlink);
    return false;
}



