/* I included all the relevant functions for what I've tried to write for bg process. 
seems to work somewhat but def has bugs */


bool is_Path(tokenlist *tokens, bool bg);         //updated to take bg flag to avoid double fork
void external_cmd(char * path, tokenlist * tokens, bool bg); 


//background processing
void check_background();           //check every active background process
                                   //print every finished background process
                                
bool run_background(tokenlist * tokens);       //if finds & token, remove and return true
bool exec_background(tokenlist * tokens);      //forks to execute in bg and updates job lists 


//global arrays for bg job info storage
pid_t bg_list [10];       //list of background pids
pid_t status_list [10];   //list of process status (done/not done) , not sure if this will work
int num_bg_jobs=0;        //number of background jobs running
char * bg_args[10];       //list of strings of bg cmd tokens to print when command finishes 

int main(int argc, char const *argv[])
{
    size_t size = 10;
    char * buffer;
    
    while (1)
    {   time(&beginning);
        
        check_background();

        PrintPrompt();
        buffer=get_input();

        tokenlist *tokens = get_tokens(buffer);

        tilda_expand(tokens);
        env_expand(tokens);

        bool is_bg=false;       //flag to indicate background process
                                //can update to include flags for io/pipes
     
        if(run_background(tokens)){
            is_bg=exec_background(tokens);

            if (!is_bg){
                continue;
            //if not the bg process, return to beginning of loop
            }
        }
        //if bg process, now executes cmd
        //deal with pipe and io tokens

        time(&end); 
        totalruntime+=difftime(end,beginning);
 
        time(&beginning);

        bool builtin=get_command(tokens);
        if (builtin==false){
            bool p=is_Path(tokens,is_bg);
        }
        time(&end);           //ends timing the shell
        time_shell();         //calculates runtime and adds to total & compares to longest running cmd
        if(exitshell(tokens)==true) 
        { 
            waitpid(-1,NULL,0);
            break;
        }
        free_tokens(tokens);
    }
    free(buffer);
    return 0;
}


bool is_Path(tokenlist * tokens,bool bg)
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


        if( access(fpath, X_OK)!=0){
            token = strtok(NULL, ":");
            continue;
        }

        external_cmd(fpath,tokens,bg);  //updated to take bg flag

        free(fpath);
        free(pathcopy);
        return true;

    }

    printf("Bash: command not found: %s\n", input);
    free(fpath);
    free(pathcopy);
    return false;
}

void external_cmd(char * path, tokenlist * tokens,bool bg){   //updated to take bg flag
    
    
    char *x[tokens->size+1];  //load path and args to be executed
    x[0]=path;

    for (int i=1;i<tokens->size;i++){
        x[i]=tokens->items[i];    
    }
    x[tokens->size]=NULL;
    
    if (bg){                  //if background process, don't fork again 
        execv(x[0],x);
    }

    int pid=fork();

    if (pid==0){
        //in child
        int e=execv(x[0],x);

    } else {
            //printf("i am a parent\n");
            waitpid(pid,NULL,0);           
    }
}

void check_background(){
//for process in list of bg processes
    pid_t new_bg_list[10];
    pid_t new_status_list[10];
    char* new_bg_args[10];
    bool completed_prev=false;
    bool update=false;

    

    for (int i = 0; i<num_bg_jobs ;i++){
        pid_t status=waitpid(bg_list[i],NULL,WNOHANG);

        if (status!=0){
           
            printf("[%i]   Done            %s\n",i+1,bg_args[i]);
            //proccess finished
            num_bg_jobs--;
            completed_prev=true;
            update=true;

        }
        else{
            int j=i;
            if (completed_prev){
                j--;
                completed_prev=false;
            }
            
            new_bg_list[j]=bg_list[i];
            new_bg_args[j]=(char*) malloc(sizeof(bg_args[i]));
            new_bg_args[j]=bg_args[i];
            new_status_list[j]=new_status_list[i];

        }
   
    }
    if (update){
        for (int i=0;i<num_bg_jobs;i++){
            bg_list[i]=new_bg_list[i];
            status_list[i]=new_status_list[i];
            bg_args[i]=new_bg_args[i];
            free(new_bg_args[i]);
        }
    }
    
    return;
     
}

bool run_background(tokenlist * tokens){
//returns true if user requested background processing 
    if (tokens->items[tokens->size-1][0]=='&'){
        tokens->items[tokens->size-1]=NULL;    //remove & token

        return true;
    }
    return false;

}

bool exec_background(tokenlist * tokens){
  
    num_bg_jobs++;
    bg_args[num_bg_jobs-1]=(char*)malloc(sizeof(tokens->items));
    strcpy(bg_args[num_bg_jobs-1],tokens->items[0]); 
    for (int i=1;i<tokens->size-1;i++){
        strcat(bg_args[num_bg_jobs-1]," ");
        strcat(bg_args[num_bg_jobs-1],tokens->items[i]);           
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
        
   
        //parent should continue to run
        pid_t status=waitpid(pid,NULL,WNOHANG);
        status_list[num_bg_jobs-1]=status;

        return false;
        //background process still running

    }

}

