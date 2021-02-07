#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <wordexp.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>

/**Provided functions and struct**/
typedef struct {
	int size;
	char **items;
} tokenlist;

char *get_input(void);
tokenlist *get_tokens(char *input);

tokenlist *new_tokenlist(void);
void add_token(tokenlist *tokens, char *item);
void free_tokens(tokenlist *tokens);

//GLOBAL VARIABLES
int ifile, ofile;
char * input;
char * output;
bool iflag, oflag = false;

/**Provided functions and struct**/






void external_cmd(tokenlist * tokens, bool bg, bool io);
bool get_command(tokenlist * tokens);

//background processing
void check_background(void);
bool run_background(tokenlist * tokens);
void update_jobs(tokenlist * tokens);


/****From External Files****/
//bool redirection(tokenlist *tokens);
bool exitshell(tokenlist *tokens);
void time_command(time_t START, time_t STOP);    
//bool redirection(tokenlist * tokens);

//file io
bool redirect_tokens(tokenlist *tokens);
void open_fd(void);
void close_fd(void);


/****Tested and work****/ /****Printing/Character Expansion Functions****/
void print_Prompt(void);
void tilde_Expand(tokenlist * tokens);
void env_Expand(tokenlist *tokens);

/**Built-Ins**/
void cd(tokenlist * tokens);
void echo(tokenlist * tokens);
void check_and_exit(void);
void jobs(tokenlist *tokens);
bool is_Path(tokenlist *tokens);


#endif
