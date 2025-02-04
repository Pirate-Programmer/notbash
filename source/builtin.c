#include "notbash.h"

int notbash_echo(char** argv)
{
    int messageIdx = 1;
    bool n_flag = false;

    //checking for -n flag
    if(argv[1] && !strcmp(argv[1],"-n"))
    {
        n_flag = true;
        messageIdx = 2;
    }
        
    //printing mssg input by user
    for(int i = messageIdx; argv[i]; i++)
    {
        p("%s",argv[i]);
        if(argv[i + 1])
            p(" ");
    }

    //if -n flag passed skip trailing new line character
    if(!n_flag)
        printf("\n");
    return EXIT_SUCCESS;
}


int notbash_exit(char** argv)
{
    p("Exiting shell ...\n");
    exit(EXIT_SUCCESS);
}

int notbash_pwd(char** argv)
{
    size_t current_dir_size = BUFSIZ;
    char current_dir[current_dir_size];
    Getcwd(current_dir,current_dir_size);
    p("%s\n",current_dir);
    return EXIT_SUCCESS;
}

int notbash_cd(char ** argv)
{
    if(!argv[1])
    {
        fprintf(stderr,RED"cd: missing path argument"RST"\n");
        return -1;
    }
    if(chdir(argv[1]) == 1)
    {
        fprintf(stderr,RED"cd failed"RST"\n");
        return -1;
    }
    return EXIT_SUCCESS;
}