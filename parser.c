#include "shell.h"



int main(int argc, char const * argv[])
{
  //Start Timer and Initialize Global Variables
  time(&SHELL_START);
  NUM_JOBS=0;
  currentruntime=0;
  longestruntime=0;
  
    while (1)
    {
      // Gets user input and checks bg jobs
      int counter = 0;
      if (counter != 0)
        check_background();
      counter++;
      
      // Presents user with prompt and runs command
      // from provided functions
      print_Prompt();
      char * buffer=get_input();
      
      check_background();
      
      // Allows NULL input in our shell
      while(buffer[0] == 0 || buffer[0] == 32)
      {
            check_background();
            print_Prompt();
            buffer=get_input();
      }
      
      time(&CMD_START);			//start cmd timing
      
      // Creates tokenlist with provided functions
      tokenlist *tokens = get_tokens(buffer);
      
      // Expands any necessary tokens within the tokenlist
      // tokens beginning with "~" or "$"
      tilde_Expand(tokens);
      env_Expand(tokens);
      
      // Reset redirect flags for each new input
      iflag = false;
      oflag = false;
      
      // Checks whether the commands require background processing, piping, or I/O redirection
      // Executes commands
      if (!get_command(tokens))
      {
        bool is_bg = run_background(tokens);
        bool is_io = redirect_tokens(tokens);
        bool is_pipe=pipe_tokens(tokens);
        
          if (is_pipe)
          {
            pipe_exec(is_bg,tokens);
          }
          else if (!is_Path(tokens))
          {
            printf("Bash: command not found: %s\n", tokens->items[0]);
          }
          else
          {
            //true if bg processing needed, removes & token
            if (is_bg)
            {
              time(&BG_STARTS[NUM_JOBS]);
              update_jobs(tokens);
            }

            external_cmd(tokens,is_bg,is_io);
          }

      }
      
      time(&CMD_STOP); 			//end cmd timing
      time_command(CMD_START,CMD_STOP); 	//update currentlongest cmd run time
      
      // End of While loop if "exit", shell ends and calculates running times
      if(exitshell(tokens) == true)
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
    if(strcmp(c, tokens->items[0]) == 0)
    { return true; }
    else
    { return false; }
}

void time_command(time_t START,time_t STOP) //checks current run time and updates longest run time
{
    currentruntime = difftime(STOP,START);
    if (longestruntime < currentruntime)
    { longestruntime = currentruntime; }

}

// Runs external commands when necessary
void external_cmd(tokenlist * tokens, bool bg, bool io)
{
    char * x[tokens->size + 1];
    x[0] = tokens->items[0];

    for (int i=1; i < tokens->size; i++)
    { x[i]=tokens->items[i]; }

    x[tokens->size] = NULL;
    
    //open files before fork
    if(iflag)
    { ifile = open(input, O_RDONLY); }
    if (oflag)
    { ofile = open(output, O_WRONLY | O_CREAT| O_TRUNC, 0777); }

    pid_t pid=fork();

    if (pid == 0)
    {
      //in child
      if (io)
      { open_fd(); }
      int e = execv(x[0], x);
    }
    
    else
    {
      if (io)
      {
        close_fd();
      }
      if (bg)
      {
        //if bg processing:update job pid list, print job, continue immediately
        BG_LIST[NUM_JOBS-1] = pid;
        printf("[%i] %i\n",NUM_JOBS,BG_LIST[NUM_JOBS-1]);
        return;
      }
        //if no bg, wait for child to finish
        waitpid(pid,NULL,0);
    }
}


// Check if command is a built-in, if so, runs the command
bool get_command(tokenlist *tokens)
{
  if (strcmp(tokens->items[0], "cd") == 0)
  {
    cd(tokens);
    return true;
  }
  
  else if (strcmp(tokens->items[0], "echo") == 0)
  {
    echo_Function(tokens);
    return true;
  }
  
  else if (strcmp(tokens->items[0],"exit") == 0)
  {
    //wait for exit function to be called later
    return true;
  }
  
  else if (strcmp(tokens->items[0], "jobs") == 0)
  {
    jobs(tokens);
    return true;
  }
  return false;
}

//Checks if a process is in the BG
void check_background()
{
  pid_t new_BG_LIST[10];
  char* new_BG_ARGS[10];
  int new_num = NUM_JOBS;
  
  int j=0;
  for (int i = 0; i < NUM_JOBS; i++)
  {
    pid_t status=waitpid(BG_LIST[i],NULL,WNOHANG);
    
    if (status!=0)
    {
      time(&BG_STOP);
      time_command(BG_STARTS[i],BG_STOP);
      printf("[%i]+  %s &\n",i+1,bg_args[i]);
      
      //proccess finished
      new_num--;
    }
    
    else
    {
      new_BG_LIST[j]=BG_LIST[i];
      new_BG_ARGS[j]=(char*) malloc(sizeof(BG_ARGS[i]));
      new_BG_ARGS[j]=BG_ARGS[i];
      j++;
    }
  }
  
  if (NUM_JOBS!=new_num)
  {
    NUM_JOBS=new_num;
    
    for (int i=0;i<NUM_JOBS;i++)
    {
      BG_LIST[i]=new_BG_LIST[i];
      BG_ARGS[i]=new_BG_ARGS[i];
    }
  }
  return;
}

bool run_background(tokenlist * tokens)
{
  //returns true if user requested background processing
  if (tokens->items[tokens->size-1][0] == '&')
  {
    tokens->items[tokens->size-1]=NULL;    //remove & token
    tokens->size--;
    return true;
  }
  return false;
}

// Updates array of jobs
void update_jobs(tokenlist * tokens)
{
    NUM_JOBS++;
    //update bg cmd line args list
    
    char * cmd_loc=strrchr(tokens->items[0],'/');

    if(cmd_loc==NULL)
    {  //if first token has not been expanded
        //add first token as is
       
        BG_ARGS[NUM_JOBS] = (char*)malloc(sizeof(tokens->items[0]));
        strcpy(BG_ARGS[NUM_JOBS-1], tokens->items[0]);
    }
    else
    {
        //if first token has been expanded, add cmd only
        cmd_loc++;
        BG_ARGS[NUM_JOBS-1] = (char*) malloc(sizeof(cmd_loc));
        strcpy(BG_ARGS[NUM_JOBS-1], cmd_loc); 
    }
  
    for (int i=1;i<tokens->size;i++)
      {
        strcat(BG_ARGS[NUM_JOBS-1], " ");
        strcat(BG_ARGS[NUM_JOBS-1], tokens->items[i]);
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
    printf("Shell ran for %.0f seconds and took %d seconds to execute one command.\n", shell_time, longestruntime);
    return;
}
