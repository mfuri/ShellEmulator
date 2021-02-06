#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <wordexp.h>
#include <fcntl.h>
#include <time.h>

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
bool is_Path(tokenlist * tokens);
char * TildeExpand(char * input);

void cd(char* path);

bool get_command(tokenlist *tokens);
void external_cmd(char * path, tokenlist * tokens);

void echo(tokenlist *tokens);
void time_shell();                                                
bool exitshell(tokenlist *tokens);   
time_t beginning, end;
int totalruntime,currentruntime, longestruntime;

bool redirection(tokenlist *tokens);
bool redflag = false;

int main(int argc, char const *argv[]){
    size_t size = 10;
    char * buffer;
    
   
    while (1)
    //do
    {   time(&beginning);
        
        PrintPrompt();
        buffer=get_input();

        tokenlist *tokens = get_tokens(buffer);

        TildeExpand(tokens);		//note: I'm using these fns that expand all tokens currently bc I was getting compiler errors 
        EnvExpand(tokens);		// when I tried to switch to using michaels fns. I think his implementation is cleaner tho, not sure 
     					//why it wasn't working for me. my versions of the expansion fns are in a diff file just to not clutter things
     					//but atm this file won't run without those functions added. 

        time(&end); 
        totalruntime+=difftime(end,beginning);
 
        time(&beginning);

        /*bool builtin=get_command(tokens);		//if not built in command, search for external path
        if (builtin==false){
            bool p=is_Path(tokens);
        }*/
       
       redirection (tokens);        

        time(&end); 
                                                                    //ends timing the shell
        time_shell();      
                                                             //calculates runtime and adds to total and compares to longest running command
        if(exitshell(tokens)==true) 
        {
            waitpid(-1,NULL,0);
            break;
        }

       /*for (int i = 0; i < tokens->size; i++) {
       printf("token %d: (%s)\n", i, tokens->items[i]);
        }*/
       
        
        free_tokens(tokens);
    }
    //while (strcmp(buffer, "exit") != 0);

    free(buffer);
    
    return 0;
}

tokenlist *new_tokenlist(void){
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

bool is_Path(tokenlist * tokens)
{
    char * input=tokens->items[0];
    char * path = getenv("PATH");

    char * pathcopy = (char *)malloc(strlen(path)+1);		//this copy might not be necessary
    strcpy(pathcopy,path);

    char * token = strtok(pathcopy,":");
    char * fpath= (char *) malloc(strlen(token)+strlen(input)+2);

    while (token != NULL)
    {   
        
        fpath=(char*) realloc(fpath,strlen(token)+strlen(input)+2);
        strcpy(fpath,token);
        strcat(fpath,"/");
        strcat(fpath,input);		//fpath = current path being checked + /cmd

        if( access(fpath, X_OK)!=0){	//if cmd file not in current path, move to next path
            //printf( " %s\n", token ); //printing each token
            token = strtok(NULL, ":"); 
        strcat(fpath,input);
        //printf("loop");
        }

        if( access(fpath, X_OK)!=0){
            //printf( " %s\n", token ); //printing each token
            token = strtok(NULL, ":");

            continue;
        }
        printf("cmd path found\n");	//if loop gets here cmd was found
        //printf("%s",fpath);		//location of cmd
        //execute command
        external_cmd(fpath,tokens); //call fn to execute cmd
        free(fpath);
        free(pathcopy);
        return true;
    }

    printf("Bash: command not found: %s\n", input); //if exits loop, command not found
    free(fpath);
    free(pathcopy);
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



bool get_command(tokenlist *tokens)
{
    if (strcmp(tokens->items[0],"cd")==0){
        cd(tokens->items[1]);
        return true;
    }

    if (strcmp(tokens->items[0],"echo")==0)
    {
      echo(tokens);
      return true;
    }

   
    if (strcmp(tokens->items[0],"exit")==0){
 
        return true;
        
    }
    printf("not a built in command");
    return false;
}
      
bool exitshell(tokenlist *tokens)                                  
{
  char* check = "exit";
	
	if(strcmp(check,tokens->items[0])==0)
	{
		printf("Shell ran for %d seconds and took %d seconds to execute one command.", totalruntime, longestruntime);
		return true;
	}
	else
	{
		return false;
	}
	
}

void time_shell()
{
	currentruntime = difftime(end,beginning);								//store command execution run time.
	totalruntime  += currentruntime;										//time storing
	if(longestruntime<currentruntime)
	{
		longestruntime = currentruntime;
	}

}

void echo(tokenlist *tokens)
{
  for (int i=1;i<tokens->size;i++)
  {
    printf("%s ",tokens->items[i]);   
  }
  printf("\n");
}

bool redirection(tokenlist *tokens)
{

  char * cmd;                                                 //string
  char * arg = (char *) malloc(strlen(tokens));
  char * input;
  char * output;

  tokenlist * argument;                                         //token list for execution

  redflag = true;                                             //flag
  bool oflag = false; 
  bool iflag = false;

  int file;
  int file2;
  
  pid_t r_pid = fork();

  if(r_pid == 0)                                                //child process
  {
    strcat(arg,tokens->items[0]);                 //add the command argument to the argument string

    for(int i = 1; i <tokens->size ; i++)
    {
      if(strcmp("<", tokens->items[i])==0)
      { 
        input = tokens->items[i+1];   
        iflag = true;
        //printf("got input: %s \n", input);
      }
      else if(strcmp(">", tokens->items[i])==0)
      {
        output = tokens->items[i+1];
        oflag = true;
        //printf("got output: %s \n", output);        
      }
      else
      {
        if(iflag == false && oflag == false){
        strcat(arg, tokens->items[i]);
        }
        continue;
      }
    }
    argument = get_tokens(arg);

    
    if(oflag = true)
    {
      file = open(output, O_WRONLY | O_CREAT| O_TRUNC, 0777);       //anything left of < is an output, so create or overwrite fil
      
      dup2(file, STDOUT_FILENO);
      //printf("about to call excec\n");
   
    }
    
    if(iflag = true)
    {
      file2 = open(input, O_RDONLY, 0777);        //anything left of < is an output, so create or overwrite file
      
      dup2(file2, STDIN_FILENO);
  
    }
    is_Path(argument);
  }

else
  {
    
    waitpid(r_pid, NULL, 0);

   close(file);
   close (file2);

    return true;
  }
  
}


void external_cmd(char * path, tokenlist * tokens)
{ 
    char *x[tokens->size+1];
    x[0]=path;
    int pid;

    for (int i=1;i<tokens->size;i++){
        x[i]=tokens->items[i];    
    }

    x[tokens->size]=NULL;
    if(redflag == false)
    {
       pid=fork();	

    }
    if (pid==0 || redflag == true){
        //in child
      printf("\n i'm in the external cmd function \n");
      execv(x[0],x);
      printf("\n this previous line is execv \n");

    } else 
    {
            //printf("i am a parent\n");
            waitpid(pid, NULL, 0);


    }
}

