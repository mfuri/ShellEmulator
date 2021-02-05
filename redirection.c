bool redirection(tokenlist *tokens)
{
  int o_stdout = dup(STDIN_FILENO);
  int o_stdin = dup(STDOUT_FILENO);
  char * cmd;
  char * input;
  bool oflag = false;
  char * output;
  bool iflag = false;
  int file;
  int file2;
  
  pid_t pid = fork();

  if(pid == 0)                                                //child process
  {   
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
        continue;
      }
    }
    
    if(oflag = true)
    {
      file = open(output, O_WRONLY | O_CREAT| O_TRUNC, 0777);       //create or overwrite fill of "output" name
      close(STDOUT_FILENO);
      dup2(file, STDOUT_FILENO);
      close(file);
    //printf("about to call is_Path\n");
    }
    
    if(iflag = true)
    {
      file = open(input, O_RDONLY);        //if file exists, it'll open successfully. Read only
      close(STDIN_FILENO);
      dup2(file2, STDIN_FILENO);
      close(file2);
    }

    is_Path(tokens);
  }

else
  {
    waitpid(-1, NULL, 0);
    int o_stdout = dup(STDIN_FILENO);
    int o_stdin = dup(STDOUT_FILENO);
    return true;
  }
  
}
