#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <wordexp.h>
#include <time.h>

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
/**Provided functions and struct**/




//bool is_Path(tokenlist *tokens);
void external_cmd(char * path, tokenlist * tokens);
bool get_command(tokenlist * tokens);


/****From External Files****/
//bool is_Path(char * input);
bool redirection(tokenlist *tokens);
bool exitshell(tokenlist *tokens);
void command_Time(void);
bool redirection(tokenlist * tokens);




/****Tested and work****/ /****Printing/Character Expansion Functions****/
void print_Prompt(void);
void tilde_Expand(tokenlist *);
void env_Expand(tokenlist *);

/**Built-Ins**/
void cd(tokenlist *);
void echo(tokenlist *);


#endif
