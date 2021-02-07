#include "shell.h"

//globals
int ifile, ofile;
char * input, output;
bool iflag, oflag = false;
tokenlist * argument;  //token list for execution


tokenlist  *redirection_tokens(tokenlist *tokens)
{
	argument = new_tokenlist();
  //tokenlist * argument = new_tokenlist();                                        //token list for execution

  //Add items to new tokenlist before < or >
  int i; 
  //CREATE NEW TOKENLIST WITHOUT FILENAMES OR I/O
  
  for(i = 0; i < tokens->size ; i++)
  {
    if(strcmp("<", tokens->items[i])==0)
    { 
      input = tokens->items[i+1];   
      iflag = true;
    }
	  
    else if(strcmp(">", tokens->items[i])==0)
    {
      output = tokens->items[i+1];
      oflag = true;
    }
    else
    {
      if(iflag == false && oflag == false)
      {
		  add_token(argument, tokens->items[i]);
      }
    }

  }
}
  
  
void open_fd();
{
  //inside fork 2nd function

  if(iflag)
  {
    if(ifile = open((input, O_RDONLY) != -1  );       //anything left of > is an output, so create or overwrite fil
    {
      dup2(ifile, STDIN_FILENO);
    }
    else
    {
      printf("Error opening or creating file.\n");
      break;
    }     
  }

  else if(oflag)
  {
    if(ofile = open(output, O_WRONLY | O_CREAT| O_TRUNC) != -1 );       //anything left of > is an output, so create or overwrite fil
    {
      dup2(ofile, STDOUT_FILENO);
    }

    else
    {
      printf("Error opening or creating file.\n");
      break;
    }
          
  }
    external_cmd(argument);
    return true;

}
// parent 
void close_fd()
{
  //wait?

  close(ifile);
  close(ofile);

  if(input != NULL)         
    free (input);
  if(output != NULL )
    free (output);
  
  iflag = false;            //reset flags
  oflag = false;
  
  free_tokens(argument);
}
