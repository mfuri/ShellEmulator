#include "shell.h"

void redirection(tokenlist *tokens)
{
  tokenlist * argument = new_tokenlist();                                         //token list for execution

  //Add items to new tokenlist before < or >
  bool iflag, oflag = false;
  char * input, output;
  int i, ifile, ofile;

  //CREATE NEW TOKENLIST WITHOUT FILENAMES OR I/O
  
  for(int i = 0; i < tokens->size ; i++)
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
  if(iflag)
  {
    if(ifile = open((input, O_RDONLY) != -1  );       //anything left of > is an output, so create or overwrite fil
    {
      dup2(ifile, STDIN_FILENO);
    }
    else
    {
      printf("Error opening or creating file\n");
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
      printf("Error opening or creating file\n");
      break;
    }
          
  }
    external_cmd(argument);
    return true;
  }

//wait?
//close?
  free (arguments);
}
