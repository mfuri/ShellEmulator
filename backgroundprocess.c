#include "shell.h"

void check_background()
{
//for process in list of bg processes
    pid_t new_bg_list[10];
    char* new_bg_args[10];
    int new_num = num_bg_jobs;

    int j=0;
    for (int i = 0; i < num_bg_jobs; i++){
        pid_t status=waitpid(bg_list[i],NULL,WNOHANG);
        if (status!=0){
            printf("[%i]   Done            %s\n",i+1,bg_args[i]);
            //proccess finished
            new_num--;

        }
 
        else{
            new_bg_list[j]=bg_list[i];
            new_bg_args[j]=(char*) malloc(sizeof(bg_args[i]));
            new_bg_args[j]=bg_args[i];
            j++;
          
            }
    }
       
    if (num_bg_jobs!=new_num){
        num_bg_jobs=new_num;

        for (int i=0;i<num_bg_jobs;i++){
            bg_list[i]=new_bg_list[i];
            
            bg_args[i]=new_bg_args[i];        
        }   
    }
    
    return;
     
}

bool run_background(tokenlist * tokens){
//returns true if user requested background processing 
    if (tokens->items[tokens->size-1][0]=='&'){
        tokens->items[tokens->size-1]=NULL;    //remove & token
        tokens->size--;
        return true;
    }
    return false;

}

bool exec_background(tokenlist * tokens)
{
  
    num_bg_jobs++;
    bg_args[num_bg_jobs-1]=(char*)malloc(sizeof(tokens->items));
    strcpy(bg_args[num_bg_jobs-1],tokens->items[0]); 
    for (int i=1;i<tokens->size;i++)
	{
        strcat(bg_args[num_bg_jobs-1]," ");
        strcat(bg_args[num_bg_jobs-1], tokens->items[i]);
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
       
        return false;
        

    }

}
