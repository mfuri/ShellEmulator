#include "shell.h"

/**GLOBAL TIME VARIABLES**/
time_t CMD_START, CMD_STOP,SHELL_START, SHELL_STOP, BG_STOP;                                             //global variables
int currentruntime = 0;
int longestruntime = 0;

/**GLOBAL PROCESS VARIABLES**/
int NUM_JOBS = 10;
pid_t BG_LIST[10];
char * BG_ARGS[10];
time_t BG_STARTS[10];

/**GLOBAL IO VARIABLES**/
int ifile, ofile;
char * input;
char * output;
bool iflag, oflag = false;

int main(int argc, char const * argv[])
{
    time(&SHELL_START);
	
    while (1)
    {	//getting user input and checking bg jobs
	    
        check_background();
        print_Prompt();

        char * buffer=get_input();

        check_background();

        while(buffer[0]==0||buffer[0]==32){
            check_background();
            print_Prompt();

            buffer=get_input();
        }

	
        time(&CMD_START);			//start cmd timing

        tokenlist *tokens = get_tokens(buffer);

        tilde_Expand(tokens);			//token expansion
        env_Expand(tokens);

        iflag=false;        //reset redirect flags for each new input
        oflag=false;

        if (!get_command(tokens)){
	    bool is_bg=run_background(tokens);
            bool is_io=redirection_tokens(tokens);

            if (!is_Path(tokens)){
                printf("Bash: command not found: %s\n", tokens->items[0]);
            }
            else{
                   //true if bg processing needed, removes & token
                if (is_bg){
                    time(&bg_starts[num_bg_jobs]);
                    update_jobs(tokens);
                }

                external_cmd(tokens,is_bg,is_io);

            }

        }
      
        time(&CMD_STOP); 			//end cmd timing
                                                           
        time_command(CMD_START,CMD_STOP); 	//update currentlongest cmd run time
              
        if(exitshell(tokens)==true) 		//if cmd is "exit"
        {   
            check_and_exit();			//waits for bg processes and prints exit statement
            break;
        }



        free(buffer);
        free_tokens(tokens);

        
    }

    return 0;
}

//function definitions
bool exitshell(tokenlist *tokens) //returns true if command is exit
{                                 
    char *c = "exit";
    if(strcmp(c,tokens->items[0])==0)
    {      
        return true;
    }
    else
    {
        return false;
    }    
}

void time_command(time_t START,time_t STOP) //checks current run time and updates longest run time
{
    currentruntime = difftime(STOP,START);

    if(longestruntime<currentruntime)
    {   
        longestruntime = currentruntime;
    }

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

        tokens->items[0] = (char*) realloc(tokens->items[0], strlen(fpath)+1);
        strcpy(tokens->items[0], fpath);

        free(fpath);
        free(pathcopy);
        return true;

    }

    
    free(fpath);
    free(pathcopy);
    return false;
}

void external_cmd(tokenlist * tokens,bool bg, bool io){ 

    char *x[tokens->size+1];
    x[0]=tokens->items[0];

    for (int i=1;i<tokens->size;i++){
        x[i]=tokens->items[i];    
    }

    x[tokens->size]=NULL;
    
    //open files before fork
    if(iflag){
        ifile = open(input, O_RDONLY);
    }
    if (oflag){
        ofile = open(output, O_WRONLY | O_CREAT| O_TRUNC);
    }

    pid_t pid=fork();

    if (pid==0){
        //in child
        if(io){
            open_fd();
        }

        int e=execv(x[0],x);

    } else {
        if(io){
            close_fd();
        }
        if(bg){
            //if bg processing:update job pid list, print job, continue immediately
            bg_list[num_bg_jobs-1]=pid;
        
            printf("[%i] %i\n",num_bg_jobs,bg_list[num_bg_jobs-1]);
            return;
        }
            //if no bg, wait for child to finish
            waitpid(pid,NULL,0);    
              
    }
}


/*
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
 	//wait for exit function to be called later
        return true;
        
    }
    if (strcmp(tokens->items[0],"jobs")==0){
        jobs(tokens);
        return true;
    }
    return false;
}*/

void check_background()
{
//for process in list of bg processes
	pid_t new_BG_LIST[10];
	char* new_BG_ARGS[10];
	int new_num = NUM_JOBS;

	int j=0;
	for (int i = 0; i < NUM_JOBS; i++){
		pid_t status=waitpid(BG_LIST[i],NULL,WNOHANG);
		if (status!=0){
			time(&BG_STOP);
            		time_command(BG_STARTS[i],BG_STOP);
			printf("[%i]   Done            %s\n",i+1,BG_ARGS[i]);
			//proccess finished
			new_num--;

		}
 
		else{
			new_BG_LIST[j]=BG_LIST[i];
			new_BG_ARGS[j]=(char*) malloc(sizeof(BG_ARGS[i]));
			new_BG_ARGS[j]=BG_ARGS[i];
			j++;
		  
			}
	}
	   
	if (NUM_JOBS!=new_num){
		NUM_JOBS=new_num;

		for (int i=0;i<NUM_JOBS;i++){
			BG_LIST[i]=new_BG_LIST[i];
			
			BG_ARGS[i]=new_BG_ARGS[i];
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

void update_jobs(tokenlist * tokens){
    
    num_bg_jobs++; 
    //update bg cmd line args list
    char * cmd_loc=strrchr(tokens->items[0],'/')+1;
    bg_args[num_bg_jobs-1]=(char*)malloc(sizeof(cmd_loc));
    strcpy(bg_args[num_bg_jobs-1],cmd_loc); 

    for (int i=1;i<tokens->size;i++){
        strcat(bg_args[num_bg_jobs-1]," ");
        strcat(bg_args[num_bg_jobs-1],tokens->items[i]);           
    }


    


}

void jobs(tokenlist *tokens){
    //output list of active background process
    //[job #]+ [CMD's pid]  [CMD's cmd line]

    for (int i=0;i<NUM_JOBS;i++){
        printf("[%i]+ %i  %s &\n",i+1,BG_LIST[i],BG_ARGS[i]);

    }
}

void check_and_exit()
{
    //waits for bg jobs to complete and exits shell
    while(NUM_JOBS!=0)
	{
         waitpid(-1,NULL,0);                 //wait for bg jobs to finish
         check_background();                 //prints any jobs that just completed/checks those run times
    }

    time_command(CMD_START,CMD_STOP);       //checks last cmd's run time
    time(&SHELL_STOP);                      //ends shell timing
    double shell_time = difftime(SHELL_STOP,SHELL_START);
    printf("Shell ran for %f seconds and took %d seconds to execute one command.\n", shell_time, longestruntime);
    return;
}





