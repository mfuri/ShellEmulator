#include "shell.h"

bool redirection(tokenlist *tokens)
{
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
  }

else
  {
    
   waitpid(r_pid, NULL, 0);
   redflag = false;
   close(file);
   close (file2);
    return true;
  }
  
}
