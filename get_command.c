#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



typedef void (*fnptr)();
fnptr get_command(tokenlist *tokens);

void commanderr();
void ex();

fnptr get_command(tokenlist *tokens){
    char * command=tokens->items[0];
    if (strcmp(command,"echo")==0)
        return echo;
    if (strcmp(command, "exit")==0)
        return ex;
    if (strcmp(command,"cd")==0)
        return cd;

    else
        return commanderr;
}
      
  
void commanderr(){
    printf("\n%s\n", "Error: command not found");
}

void ex(){
    //check for background process
    //implement timer stuff
    printf("\n%s\n", "Shell ran for ...");
}
