#include "notbash.h"


typedef struct builtin{
    const char* cmd;
    int (*foo)(char**);
}builtin;

builtin func[] = {
    {.cmd = "echo", .foo = notbash_echo},
    {.cmd = "exit", .foo = notbash_exit},
    {.cmd = "pwd", .foo = notbash_pwd},
    {.cmd = "cd", .foo = notbash_cd},
    {.cmd = NULL}
};


//WRAPPERS

//bug: on pressing ctrl+d stops reading and prints eof endlessly
//soln as of not exit on eof true
void Getline(char **buffer, size_t *buff_size, FILE *stream)
{
    if(!buffer || !stream)
    {
        p(RED"invalid aruments"RST"\n");
        return;
    }
    if(getline(buffer, buff_size, stream) == -1)
    {
         //error or eof when return value -1 then free buffer
        free(*buffer);
        *buffer = NULL;
        if(feof(stream))
        {
            p(RED"EOF"RST"\n");
            exit(404);
        }
        else
            fprintf(stderr,RED"getline failled"RST"\n");
    }
}

void Getcwd(char* cwd, size_t cwd_size)
{
    getcwd(cwd,cwd_size);
    if(NULL == cwd)
        fprintf(stderr,RED"getcwd failed"RST"\n");
}

void* Malloc(size_t size)
{
    void* temp = malloc(size);
    if(NULL == temp)
    {
        fprintf(stderr,RED"malloc error"RST"\n");
        exit(EXIT_FAILURE);
    }
    return temp;
}

void* Realloc(void* ptr,size_t size)
{
    ptr = realloc(ptr,size);
    if(NULL == ptr)
    {
        fprintf(stderr,RED"realloc error"RST"\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

int Fork()
{
    pid_t pid = fork();
    //fork failed
    if(pid < 0) 
    {
        fprintf(stderr,RED"fork failed"RST"\n");
        exit(EX_OSERR);
    }
    return pid; //this is the pid of child process 
}


void Execvp(const char* file, char* const argv[])
{
    if(!file && !argv)
    {
        fprintf(stderr,RED"execvp: Invalid arguments"RST"\n");
        exit(EXIT_FAILURE); //exiting out of child process
    }
    if(execvp(file,argv) == -1)
    {
        fprintf(stderr,RED"lil_Buddy Failed"RST"\n");
        exit(EX_UNAVAILABLE); //exiting out of child process
    }
}

pid_t Wait(int* status)
{
    pid_t result;
    if(!status)
    {
        fprintf(stderr,RED"wait status argument missing"RST"\n");
        return EXIT_FAILURE;
    }
    result = wait(status);
    if(result == -1)
    {
        fprintf(stderr,RED"wait failed"RST"\n");
    }
    if(WIFEXITED(*status))
        *status = WEXITSTATUS(*status);

    return result;
}


//FUNCTIONS

//banner for notbash shell
void printbanner(){
    FILE* fptr;
    fptr = fopen("./source/banner.txt","r");
    if(NULL == fptr)
        printf(RED"Errroroor Banner.txt not found"RST"\n");
    else
    {
        size_t buff_size = BUFSIZ;
        char buff[buff_size];

        while(fgets(buff,buff_size,fptr) != NULL)
        {
            p(GRN"%s"RST,buff);
        }
        printf("\n");
    }
    fclose(fptr);
}

//mimicing a shell prompt 
void cli_prompt(void)
{
    //get current working dir
    size_t cwd_size = BUFSIZ;
    char cwd[cwd_size];
    Getcwd(cwd,cwd_size); 
        
    //get current username
    //getlogin statically allocates memory
    //overwrites on subsequent calls
    char* user = getlogin();  
    if(NULL == user)
        fprintf(stderr,RED"getlogin FAILED"RST);

    //prompt
    p("%s@%s $> ",user,cwd);
}



//reads input using getline
//allocates memory
//buffer needs to be freed by user
char* notbash_read_line()
{
    char* buffer = NULL;
    size_t buffsize;
    Getline(&buffer, &buffsize,stdin);
   
    return buffer;
}

//parsing user input
//ie ls -al in form of
//args[0] = ls
//args[1] = -al
char** parsing(char* input)
{
    int i = 0;
    size_t tokens_size = BUFSIZ;
    char **tokens = (char** )Malloc(tokens_size * sizeof(*tokens)); //assignig 1024 ptrs
    for(char* token = strtok(input,DEL);token; token = strtok(NULL,DEL))
    {
        tokens[i++] = token;
        if(i >= tokens_size)
        {
            tokens_size *= 2; //doubling the size
            tokens = (char**)Realloc(tokens,tokens_size * sizeof(*tokens));
        }
    }

    tokens[i] = NULL; //where parsed input ends in tokens
    return tokens;
}


//eval step
//either a builtin then check for it and exec apporiate func
//else fork and call execvp
void notbash_execvp(char** argv)
{
    int status;
    if(Fork() == lil_Buddy)
    {
        Execvp(argv[0],argv);
    }
    else{
        Wait(&status);
    }
}

void notbash_exec(char** argv)
{
    int status;
    //checking if its builtin
     const char *curr = NULL;
    int i = 0;
    while(curr = func[i].cmd)
    {
        if(!strcmp(argv[0],curr))
        {
            status = func[i].foo(argv);
            return;
        }
        i++;
    }

    //fork + execvp variant
    notbash_execvp(argv);
}