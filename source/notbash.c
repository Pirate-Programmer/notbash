#include "notbash.h"

//REPL
//read -> eval -> print -> loop
//working of a simple shell








int main()
{
    char *line = NULL;
    printbanner();

    while(YOO)
    {
        //Step1: prompt + read line
        cli_prompt();
        line = notbash_read_line();

        //Step2: parsing user input
        char** argv = parsing(line);

        // int i = 0;
        // for(char* token = args[0]; token; token = args[++i])
        // {
        //     printf("%s \n",token);
        // }

        //Step3: eval
       notbash_exec(argv);
        


        //freeing input line and input parsed array
        if(line)free(line);
        if(argv)free(argv);
    }

    return EXIT_SUCCESS;
}


