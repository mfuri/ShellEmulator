#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <wordexp.h>

typedef struct {
	int size;
	char **items;
} tokenlist;

typedef struct {
    char * cmd;
    char * arg;
    char * src;
    char * dest;
} command;

char *get_input(void);
tokenlist *get_tokens(char *input);

tokenlist *new_tokenlist(void);
void add_token(tokenlist *tokens, char *item);
void free_tokens(tokenlist *tokens);


void PrintPrompt();
char * EnvExpand(char * input);
bool is_Path(char *);
char * TildeExpand(char * input);

void cd(tokenlist *tokens)
typedef void (*fnptr)();
void get_command(tokenlist *tokens);

void commanderr();
void ex();



int main()
{
	do {
		PrintPrompt();

		/* input contains the whole command
		 * tokens contains substrings from input split by spaces
		 */

		char *input = get_input();
		printf("whole input: %s\n", input);

		tokenlist *tokens = get_tokens(input);
		for (int i = 0; i < tokens->size; i++) {
			printf("token %d: (%s)\n", i, tokens->items[i]);
		}
		
		getcommand(tokens); //works for echo and cd 

		//Testing functions
		is_Path(tokens->items[0]);
		printf("%s%s", TildeExpand("$PWD"), EnvExpand("$PPP"));
		
		free(input);
		free_tokens(tokens);
	}while(strcmp(input, "exit") != 0);

    	free(input);

	return 0;
}

tokenlist *new_tokenlist(void)
{
	tokenlist *tokens = (tokenlist *) malloc(sizeof(tokenlist));
	tokens->size = 0;
	tokens->items = (char **) malloc(sizeof(char *));
	tokens->items[0] = NULL; /* make NULL terminated */
	return tokens;
}

void add_token(tokenlist *tokens, char *item)
{
	int i = tokens->size;

	tokens->items = (char **) realloc(tokens->items, (i + 2) * sizeof(char *));
	tokens->items[i] = (char *) malloc(strlen(item) + 1);
	tokens->items[i + 1] = NULL;
	strcpy(tokens->items[i], item);

	tokens->size += 1;
}

char *get_input(void)
{
	char *buffer = NULL;
	int bufsize = 0;

	char line[5];
	while (fgets(line, 5, stdin) != NULL) {
		int addby = 0;
		char *newln = strchr(line, '\n');
		if (newln != NULL)
			addby = newln - line;
		else
			addby = 5 - 1;

		buffer = (char *) realloc(buffer, bufsize + addby);
		memcpy(&buffer[bufsize], line, addby);
		bufsize += addby;

		if (newln != NULL)
			break;
	}

	buffer = (char *) realloc(buffer, bufsize + 1);
	buffer[bufsize] = 0;

	return buffer;
}

tokenlist *get_tokens(char *input)
{
	char *buf = (char *) malloc(strlen(input) + 1);
	strcpy(buf, input);

	tokenlist *tokens = new_tokenlist();

	char *tok = strtok(buf, " ");
	while (tok != NULL) {
		add_token(tokens, tok);
		tok = strtok(NULL, " ");
	}

	free(buf);
	return tokens;
}

void free_tokens(tokenlist *tokens)
{
	for (int i = 0; i < tokens->size; i++)
		free(tokens->items[i]);

	free(tokens);
}

void PrintPrompt()
{
	int host;
	char hostname[256];
	char * user = getenv("USER");
	char * pwd = getenv("PWD");
	host = gethostname(hostname, sizeof(hostname));
	
		
	if (host == -1)
		printf("Error, hostname not defined\n");

	printf("%s@%s : %s > ",user,hostname,pwd);
}

char * EnvExpand(char * in)
{
	char * output;
	if (in[0] == '$')
	{
		in++;
		output = getenv(in);
	}
	else
	    return in;
	if (output == NULL)
	    return in;
	return output;
}

//WIP, have to create a list of the tokens and then search through each item in the list to decide if file is in path
bool is_Path(char * input)
{
	//char ** path_list;
	char * path = getenv("PATH");
	char * token = strtok(path,":");

    while (token != NULL)
    {
        printf( " %s\n", token ); //printing each token
        token = strtok(NULL, ":");
    }

    printf("Bash: command not found: %s\n", input);
    return false;
}

//I believe this works
char * TildeExpand(char * input)
{
    if (input[0] != '~')
        return input;
    wordexp_t p;
    int size;
    wordexp(input, &p, 0);
    size = strlen(p.we_wordv[0]);
    char output[size+1];
    strcpy(output, p.we_wordv[0]);
    wordfree(&p);
    return output;
}


void cd(char * path){
    if (path==NULL){
        path=getenv("HOME");
    }
    if (chdir(path)==0){
        char *cwd = getcwd(NULL,0);
        setenv("PWD",cwd,1);
        free(cwd);
        return;
   }
   printf("Error");
   return;
    
    }



void get_command(tokenlist *tokens){
    if (strcmp(tokens->items[0],"cd")==0){
        cd(EnvExpand(TildaExpand(tokens->items[1])));
    }

    else if (strcmp(tokens->items[0],"echo")==0){
        echo(tokens);
    }

   
    else
        commanderr();
    return;
}
      

      
  
void commanderr(){
    printf("\n%s\n", "Error: command not found");
}

void ex(){
    //check for background process
    //implement timer stuff
    printf("\n%s\n", "Shell ran for ...");
}

void echo(tokenlist *tokens){
    
    for (int i=1;i<tokens->size;i++){
        printf("%s ",TildeExpand(EnvExpand(tokens->items[i])));   //added double expand function calls, same notes as in cd
    }
    printf("\n");
  }
