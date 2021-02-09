#include "shell.h"

bool redirect_tokens(tokenlist *tokens)
{
  int stop; //finds end point of cmd and arg tokens
	
  for(int i = 0; i < tokens->size ; i++)
  {
    if(strcmp("<", tokens->items[i])==0)
    { 
		input = (char*) malloc(sizeof(char) * (strlen(tokens->items[i+1]) +1));
      strcpy(input,tokens->items[i+1]);
      
      iflag = true;
      if (!oflag){
        stop=i;
       }
    }
      
    else if(strcmp(">", tokens->items[i])==0)
    {
		output=(char*)malloc(sizeof(char) * (strlen(tokens->items[i+1]) +1));
		strcpy(output,tokens->items[i+1]);
      oflag = true;
      
      if (!iflag){
        stop=i;
      }
    }
    
  }
  
  if (iflag || oflag){
    tokens->items[stop]=NULL;		//if io, update tokenlist accordingly
    tokens->size=stop;
   
    return true;
  }
  return false;


}

void open_fd()
{
  //inside fork chld

  if(iflag)
  {
    if(ifile != -1 )       //anything left of > is an output, so create or overwrite fil
    {
          dup2(ifile, STDIN_FILENO);
          close(ifile);
    }
    else
    {
      printf("Error opening or creating file.\n");
      free(input);
      exit(1);
    }     
  }
  if(oflag)
  {
    
    if(ofile != -1)     //anything left of > is an output, so create or overwrite fil
    {
        dup2(ofile, STDOUT_FILENO);
        close(ofile);
    }

    else
    {
      printf("Error opening or creating file.\n");
      free(output);
      exit(1);
    }
          
  }
}
// parent 
void close_fd()
{


    if(iflag){
        
        free(input);
        if (ifile!=-1){
          close(ifile);
        }
    }

    if (oflag){
        
        free(output);
        if(ofile!=-1){
          close(ofile);
        }
    }
  
}
