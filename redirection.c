#include "shell.h"

void redirection(tokenlist *tokens)
{
  tokenlist * argument = new_tokenlist();                                         //token list for execution

  //Add items to new tokenlist before < or >
  bool iflag1, iflag2, oflag1, oflag2 = false;
  char * file1, file2;
  FILE * fpointer1, fpointer2;

  int i;

  //CREATE NEW TOKENLIST WITHOUT FILENAMES OR I/O
  for (i = 0; i < tokens->size; i++)
  {
    if (strcmp('<', tokens->items[i]) == 0 || strcmp('<', tokens->items[i]) == 0 || tokens->items[i] == NULL)
      break;
    add_token(argument, tokens->items[i]);
  }

  if (strcmp('<', tokens->items[i]) == 0)
    iflag1 = true;
  elseif (strcmp('>', tokens->items[i]) == 0)
    oflag1 = true;
  
  i++;
  file1 = (char *) malloc(strlen(tokens->items[i]));
  strcpy(file1,tokens->items[i]);
  i++;
   if (strcmp('<', tokens->items[i]) == 0)
    iflag2 = true;
  elseif (strcmp('>', tokens->items[i]) == 0)
    oflag2 = true;
  i++;
  if (iflag2 || oflag2)
  {
    file2 = (char *) malloc(strlen(tokens->items[i]));
    strcpy(file2,tokens->items[i]);
  }
  bool oflag = false; 
  bool iflag = false;
  
  pid_t r_pid = fork();

  if(r_pid == 0)                                                //child process
  {
    if()
    {
        if(iflag2)
        {
          fpointer = fopen(file2, O_WRONLY | O_CREAT| O_TRUNC);
          fpointer2 = fopen()
          //fopen1
          //fopen2
          //if (filepointer1 == NULL || filepointer2 == NULL)
          // error message
           // break:
           //spawning of the processes or whathaveyous
        }
        else if(oflag2)
        {
          if(open(file2, O_WRONLY | O_CREAT| O_TRUNC) != -1   );       //anything left of > is an output, so create or overwrite fil
          {
            dup2(file2, STDOUT_FILENO);
          }
         
          printf("Error opening or creating file\n");
          break;
          //else
          //  error and break;
          
        }

        if(iflag1)
        {
          
        }
        else if(oflag1)
        {
          f_output = open(file2, O_WRONLY | O_CREAT| O_TRUNC, 0777);       //anything left of < is an output, so create or overwrite fil
        }

    }

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
      file1 = open(file1, O_WRONLY | O_CREAT| O_TRUNC, 0777);       //anything left of < is an output, so create or overwrite fil

      // 
      // iff(open(file1, O_WRONLY | O_CREAT| O_TRUNC, 0777) == 0)
      //{
            //enter code here
      //}
      
      dup2(file, STDOUT_FILENO);
      //printf("about to call excec\n");
    }
    //cat < test1 > test2
    //cat test1 > test2 < test3

    if(iflag = true)
    {
      file2 = open(input, O_RDONLY, 0777);        //anything left of < is an output, so create or overwrite file
      
      dup2(file2, STDIN_FILENO);
  
    }
  }

else
  {
    
   waitpid(r_pid, NULL, 0);
   
   close(file);
   close (file2);
    return true;
  }
  free (arguments);
}
