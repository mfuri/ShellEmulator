#include "shell.h"

char * EnvExpand(char * input);

bool is_Path(tokenlist *tokens);
void external_cmd(char * path, tokenlist * tokens);


void cd(tokenlist *tokens)

bool get_command(tokenlist *tokens);




int main(int argc, char const *argv[])
{
    size_t size = 10;
    char * buffer;
    
   
    while (1)
    //do
    {   time(&beginning);
        
        PrintPrompt();
        buffer=get_input();

        tokenlist *tokens = get_tokens(buffer);

        tilda_expand(tokens);		//note: I'm using these fns that expand all tokens currently bc I was getting compiler errors 
        env_expand(tokens);		// when I tried to switch to using michaels fns. I think his implementation is cleaner tho, not sure 
     					//why it wasn't working for me. my versions of the expansion fns are in a diff file just to not clutter things
     					//but atm this file won't run without those functions added. 

        time(&end); 
        totalruntime+=difftime(end,beginning);
 
        time(&beginning);

        bool builtin=get_command(tokens);		//if not built in command, search for external path
        if (builtin==false){
            bool p=is_Path(tokens->items[0]);
        }
     
        //
        

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

void external_cmd(char * path, tokenlist * tokens){ 

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
    if (strcmp(tokens->items[0],"cd")==0){
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
      

      

      




void echo(tokenlist *tokens){
    
    for (int i=1;i<tokens->size;i++){
        printf("%s ",tokens->items[i]);   
    }
    printf("\n");
  }
