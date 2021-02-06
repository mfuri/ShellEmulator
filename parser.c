#include "shell.h"

bool is_Path(tokenlist *tokens);
void external_cmd(char * path, tokenlist * tokens);

bool get_command(tokenlist *tokens);




int main(int argc, char const *argv[])
{
    size_t size = 10;
    char * buffer;
    
	/*TIME BEGIN FOR WHOLE SHELL**/
	BEGIN = time(NULL);
   
    while (1)
    //do
    {
        PrintPrompt();
        buffer = get_input();
		
		/**TIME BEGIN FOR EACH COMMAND**/
		COMMAND_BEGIN = time(NULL);
		
        tokenlist *tokens = get_tokens(buffer);

        tilde_Expand(tokens);
        env_Expand(tokens);

        time(&end); 
        totalruntime+=difftime(end,beginning);
 
        time(&beginning);

        bool builtin=get_command(tokens);		//if not built in command, search for external path
        if (builtin==false){
            bool p=is_Path(tokens->items[0]);
        }
     
		/**TIME END FOR EACH COMMAND**/
		COMMAND_END = time(NULL);
        
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

bool is_Path(tokenlist * tokens)
{
    char * input=tokens->items[0];
    char * path = getenv("PATH");

    char * pathcopy = (char *)malloc(strlen(path)+1);
    strcpy(pathcopy,path);

    char * token = strtok(pathcopy,":");

    char * fpath= (char *) malloc(strlen(token)+strlen(input)+2);

    while (token != NULL)
    {   
        fpath=(char*) realloc(fpath,strlen(token)+strlen(input)+2);
        strcpy(fpath,token);
        strcat(fpath,"/");
        strcat(fpath,input);
        //printf("loop");

        if( access(fpath, X_OK)!=0){
            //printf( " %s\n", token ); //printing each token
            token = strtok(NULL, ":");
            
            continue;
        }

        external_cmd(fpath,tokens); //call fn to execute cmd

        free(fpath);
        free(pathcopy);
        return true;

    }

    printf("Bash: command not found: %s\n", input);
    free(fpath);
    free(pathcopy);
    return false;
}

void external_cmd(char * path, tokenlist * tokens)
{

    char *x[tokens->size+1];
    x[0]=path;

    for (int i=1;i<tokens->size;i++){
        x[i]=tokens->items[i];    
    }

    x[tokens->size]=NULL;
 
    int pid=fork();	

    if (pid==0){
        //in child
        int e=execv(x[0],x);

    } else {
            //printf("i am a parent\n");
            waitpid(pid,NULL,0);
           

    }
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



bool get_command(tokenlist *tokens){
    if (strcmp(tokens->items[0],"cd") == 0)
	{
        cd(tokens->items[1]);
        return true;
    }

    if (strcmp(tokens->items[0],"echo")==0){
        echo(tokens);
        return true;
    }

   
    if (strcmp(tokens->items[0],"exit")==0){
 
        return true;
        
    }
    printf("not a built in command");
    return false;
}
