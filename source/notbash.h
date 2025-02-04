#ifndef NOTBASH_H
#define NOTBASH_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sysexits.h>

#define YOO 111
#define lil_Buddy 0
#define p(...) printf(__VA_ARGS__)

#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define RST "\e[0m"
#define DEL "\n\t\f\v \r"






void Getline(char **buffer, size_t *buff_size, FILE *stream);
void Getcwd(char* ,size_t);
void* Malloc(size_t);
void* Realloc(void* , size_t);
int Fork();
void Execvp(const char* ,char* const argv[]);
pid_t Wait(int * status);

void printbanner();
void cli_prompt(void);
char* notbash_read_line(void);

char** parsing(char* input);

void notbash_exec(char** );
void notbash_execvp(char** );

int notbash_echo(char**);
int notbash_exit(char**);
int notbash_pwd(char** );
int notbash_cd(char** );
#endif
