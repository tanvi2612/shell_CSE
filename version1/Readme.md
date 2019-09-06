# C-Shell
Bash Shell in C with features like getting process info given paid, background foreground processes, etc.

This is a Linux shell written in C with basic functionalities like inbuilt commands (eg. `ls` `cd`, `echo` and `pwd`) along with user-defined functions `pinfo` and normal terminal commands that are executed in it. It has the capability to run both background and foreground processes. Also takes in semicolon separated commands. Appropriate signal handling has also been added.

## RUNNING

- run `make`

- execute by using `./a.out`

- type `quit` or `exit` to exit the shell

## FILES
- shell.c : main file which contains shell loop

- cd.h - changes directory
- ls.h- displays list of all files and directories 
- echo.h- echoes whatever the user typed
- pwd.h - displays present working directory
- pinfo.h - prints the process related info of your shell program or given process ID
- prototype.h - contains all the functions called throughout the program
## Flags that can be used with commands

## CD
- “..” - moves to the previous directory  relative to the current 
- “.” - stays on the current directory
- “/” ‘- takes you to the root
- “” - reach the home directory (~)

##LS
- -l - gives a detailed list of files and directories 
- -a - shows the hidden files also 
- -la/al - the details of hidden files are also shown
