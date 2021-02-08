#include "shell.h"

//globals
//int ifile, ofile;
//char * input, * output;
//bool iflag, oflag = false;


bool redirect_tokens(tokenlist *tokens)
{
  int stop; //finds end point of cmd and arg tokens
  
  for(int i = 0; i < tokens->size ; i++)
  {
    if(strcmp("<", tokens->items[i])==0)
    { 
      input=(char*)malloc(sizeof(tokens->items[i+1]));  
      input = tokens->items[i+1];  
      
      iflag = true;
      if (!oflag){
        stop=i;
       }
    }
      
    else if(strcmp(">", tokens->items[i])==0)
    {
      output=(char*)malloc(sizeof(tokens->items[i+1]));  
      output = tokens->items[i+1];
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
    }
    else
    {
      printf("Error opening or creating file.\n");
    }     
  }
  if(oflag)
  {
    
    if(ofile != -1)     //anything left of > is an output, so create or overwrite fil
    {
        dup2(ofile, STDOUT_FILENO);
    }

    else
    {
      printf("Error opening or creating file.\n");
    }
          
  }
    
    //return true;
}
// parent 
void close_fd()
{


    if(iflag){
        close(ifile);
    }

    if (oflag){
        close(ofile);
    }
  
}
