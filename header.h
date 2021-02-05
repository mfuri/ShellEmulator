#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <wordexp.h>
#include <time.h>

typedef struct {
	int size;
	char **items;
} tokenlist;

char *get_input(void);
tokenlist *get_tokens(char *input);

tokenlist *new_tokenlist(void);
void add_token(tokenlist *tokens, char *item);
void free_tokens(tokenlist *tokens);


bool is_Path(tokenlist *tokens);
void external_cmd(char * path, tokenlist * tokens);
void cd(tokenlist *tokens);
bool get_command(tokenlist *tokens);


/****From External Files****/
void cd(char * path);
void echo(tokenlist *tokens);
void env_expand(tokenlist *tokens);
bool is_Path(char * input);
bool redirection(tokenlist *tokens);
void tilda_expand(tokenlist *tokens);




/****Tested and work****/ /****Printing/Character Expansion Functions****/
void PrintPrompt(void);
char * EnvExpand(char * input);
char * TildeExpand(char * input);