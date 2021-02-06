#include "shell.h"

bool is_Path(tokenlist *tokens);
void external_cmd(char * path, tokenlist * tokens);

bool get_command(tokenlist *tokens);

/**GLOBAL TIME VARIABLES**/
time_t BEGIN, END, COMMAND_BEGIN, COMMAND_END;
double CURRENT_RUN_TIME, TOTAL_RUN_TIME, LONGEST_RUN_TIME;

/**GLOBAL PROCESS VARIABLES**/
int num_bg_jobs;

int main(int argc, char const * argv[])
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
	//prevents seg fault if user hits enter or space enter
        while(buffer[0]==0||buffer[0]==32){
            PrintPrompt();

            buffer=get_input();

        }
		/**TIME BEGIN FOR EACH COMMAND**/
		COMMAND_BEGIN = time(NULL);
		
        tokenlist *tokens = get_tokens(buffer);

        tilde_Expand(tokens);
        env_Expand(tokens);

        bool builtin=get_command(tokens);		//if not built in command, search for external path
        if (builtin==false){
            //bool p=is_Path(tokens->items[0]);
        }
     
		/**TIME END FOR EACH COMMAND**/
		COMMAND_END = time(NULL);
        
																//ends timing the shell
		command_Time();
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

//function definitions
bool exitshell(tokenlist *tokens)
{
    char *c = "exit";
    if(strcmp(c,tokens->items[0])==0)
	{
		printf("Shell ran for %d seconds and took %d seconds to execute one command.", TOTAL_RUN_TIME, LONGEST_RUN_TIME);
		return true;
	}
	else
	{
		return false;
	}
}

void command_Time()
{
    double temp;
    temp = difftime(COMMAND_END, COMMAND_BEGIN);
    if (LONGEST_RUN_TIME < temp)
        LONGEST_RUN_TIME = temp;
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

void check_background()
{
//for process in list of bg processes
	pid_t new_bg_list[10];
	char* new_bg_args[10];
	int new_num = num_bg_jobs;

	int j=0;
	for (int i = 0; i < num_bg_jobs; i++){
		pid_t status=waitpid(bg_list[i],NULL,WNOHANG);
		if (status!=0){
			printf("[%i]   Done            %s\n",i+1,bg_args[i]);
			//proccess finished
			new_num--;

		}
 
		else{
			new_bg_list[j]=bg_list[i];
			new_bg_args[j]=(char*) malloc(sizeof(bg_args[i]));
			new_bg_args[j]=bg_args[i];
			j++;
		  
			}
	}
	   
	if (num_bg_jobs!=new_num){
		num_bg_jobs=new_num;

		for (int i=0;i<num_bg_jobs;i++){
			bg_list[i]=new_bg_list[i];
			
			bg_args[i]=new_bg_args[i];
		}
	}
	
	return;
	 
}

bool run_background(tokenlist * tokens){
//returns true if user requested background processing
	if (tokens->items[tokens->size-1][0]=='&'){
		tokens->items[tokens->size-1]=NULL;    //remove & token
		tokens->size--;
		return true;
	}
	return false;

}

bool exec_background(tokenlist * tokens)
{
  
	num_bg_jobs++;
	bg_args[num_bg_jobs-1]=(char*)malloc(sizeof(tokens->items));
	strcpy(bg_args[num_bg_jobs-1],tokens->items[0]);
	for (int i=1;i<tokens->size;i++)
	{
		strcat(bg_args[num_bg_jobs-1]," ");
		strcat(bg_args[num_bg_jobs-1], tokens->items[i]);
	}

	pid_t pid=fork();
	
	if (pid==0){
		//execute in child
		return true;

		//will go back to main loop to execute
	}
	else{

		bg_list[num_bg_jobs-1]=pid;
		
		printf("[%i] %i\n",num_bg_jobs,bg_list[num_bg_jobs-1]);
	   
		return false;
		

	}

}
