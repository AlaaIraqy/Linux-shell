#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void printDir()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s  ", cwd);
}

char **get_input(char *input)
{
    char **command = malloc(8 * sizeof(char *));
    char *parsed;
    int i = 0;

    parsed = strtok(input, " ");
    while (parsed != NULL)
    {
        command[i] = parsed;
        i++;

        parsed = strtok(NULL, " ");
    }

    command[i] = NULL;
    return command;
}


int main()
{
    char **command;
    char input[50];
    pid_t child_pid;
    int stat_loc;


    while (1)
    {
        int background = 0;
        printDir();
        gets(input);
        command = get_input(input);
        if (strcmp(command[0], "exit") == 0)
        {
            exit(0);
        }
        if (command[1]!=NULL)
        {
            if (strcmp(command[1],"&")==0)
            {
                background=1;
            }
        }
        if (background==0)
        {
            child_pid = fork();
            if (child_pid == 0)
            {
                if(execvp(command[0], command)==-1)
                {
                    printf("Execution error");
                }
                execvp(command[0], command);
            }
            else
            {
                waitpid(child_pid, &stat_loc, WUNTRACED);
            }
        }

        if (background==1)
        {
            command[1]=NULL;
            child_pid = fork();
            if (child_pid == 0)
            {
                if(execvp(command[0], command)==-1)
                {
                    printf("Execution error");
                }
                execvp(command[0], command);
            }
        }

    }
    return 0;
}

