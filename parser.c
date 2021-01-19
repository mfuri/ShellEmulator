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
fnptr get_command(tokenlist *tokens);

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


void cd(tokenlist *tokens){.    //Does not currently account for ".." 
				//updated to show full path after change and handle different input formats
    char * path;
    char * pwd;
    char * home;
 
    tokens->items[1]=TildeExpand(EnvExpand(tokens->items[1]));  //this line added to expand path within cd fn using Expand fns on single token 
			   				//haven't tested this line but I think it should work?
			   				//alternatively - expand in main with _expand on all tokens 
			   				//(this I have tested and it works for cd and echo)
			  
    pwd=getenv("PWD");
    home=getenv("HOME");
    			   
    if (tokens->size==1){                               //If no argument, cd to home
        path= (char*)malloc(strlen(home)+1);
        strcpy(path,home);            
    }
    else {                                              //otherwise get arg
        path= (char *) malloc(strlen(tokens->items[1])+1);
        strcpy(path, tokens->items[1]);
              
        if (strstr(path,home)==NULL){                   //if not a complete path, add home to start of path
            path = (char*) realloc(path, sizeof(path)+strlen(home)+1);
            strcpy(path,home);
            strcat(path,"/");
            strcat(path,tokens->items[1]);
                 
        }
        if (access(path,X_OK)!=0){
            printf("\n%s\n", "Directory not found");
             printf("\n%s\n", path);
            return;                                 //print error and exit if path not an accesible directory
        }
    }
    strcpy(pwd,path);           //change cwd
    free(path);  
    return;
}



fnptr get_command(tokenlist *tokens){ //make a list of built in command options to iterate through rather than hard code below
    char * command=tokens->items[0];
    if (strcmp(command,"echo")==0)
        return echo;
    if (strcmp(command, "exit")==0)
        return ex;
    if (strcmp(command,"cd")==0)
        return cd;

    else
        return commanderr;
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
