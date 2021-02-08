
#include "shell.h"

//piping global vars
char * CMDS[3];
int num_pipes=0;



bool pipe_tokens(tokenlist *tokens){    
//adds each command to global list of cmd line strings
//return true if needs piping
//call in main loop before calling pipe_exec
    
    for (int i=0;i<3;i++){
       CMDS[i]=(char*)malloc(sizeof(tokens->items)); 
    }
    bool new_cmd=true;

    bool ret=false;;

    for (int i=0;i<tokens->size;i++){
        if (strcmp("|",tokens->items[i])==0){
            num_pipes++;
            new_cmd=true;
            ret=true;
        }
        else{
            if (new_cmd){
                strcpy(CMDS[num_pipes],tokens->items[i]);
            }
            else{
                strcat(CMDS[num_pipes]," ");
                strcat(CMDS[num_pipes],tokens->items[i]);
            }
            
            
            new_cmd=false;
        }
     }   
    
    return ret;

}

void pipe_exec(bool bg,tokenlist * tokens)
{
    tokenlist * cmd_list[num_pipes+1];    
    
    //get paths and list of cmd tokenlists
    for (int i=0; i<num_pipes+1; i++){
        tokenlist * cmd= get_tokens(CMDS[i]);
        if (!is_Path(cmd)){
            printf("Bash: command not found: %s",cmd->items[0]);
            return;
        }
        cmd_list[i]=cmd;
    }
    //support bg processing
    if (bg){
        time(&bg_starts[num_bg_jobs]);
        
        update_jobs(tokens);
    }

    //make 2 pipes
    int pfds1[2];
    int pfds2[2];
    pipe(pfds1);
    pipe(pfds2);


    pid_t pid1=fork();
    if (pid1==0){
        //first command
        close(pfds1[0]);    //close input end of first pipe
        
        close(1);           //close std out
        dup(pfds1[1]);      //redirect to  output end of first pipe
        close(pfds1[1]);    //close output first pipe

        close(pfds2[0]);    //close 2nd pipe
        close(pfds2[1]);

        execv(cmd_list[0]->items[0],cmd_list[0]->items); //execute first cmd
        exit(1);
    }

    pid_t pid2=fork();
    if (pid2==0){
        //2nd cmd
        
        if (num_pipes==2){      //if 3rd command exists, set std.out to 2nd pipes output
            close(1);
            dup(pfds2[1]);
        }

        close(0);            //close std in
        dup(pfds1[0]);      //redirect input to input end of first pipe
        

        close(pfds1[0]);    //close both pipes
        close(pfds1[1]);

        close(pfds2[0]);
        close(pfds2[1]);

        execv(cmd_list[1]->items[0],cmd_list[1]->items);  //execute second cmd

        exit(1);
    }
   

    pid_t pid3;
    if (num_pipes==2){        //if 3rd command  

        pid3 = fork();
        if (pid3==0){       //3rd cmd
          
            close(0);       //close stdin
            dup(pfds2[0]);  //get input from input of second pipe
            
                       
            close(pfds1[0]);    //close both pipes
            close(pfds1[1]);

            close(pfds2[0]);
            close(pfds2[1]);


            execv(cmd_list[2]->items[0],cmd_list[2]->items);
            exit(1);
        }
        
     
      

        
    }
    
    close(pfds1[0]);       //close first pipe
    close(pfds1[1]);
    close(pfds2[0]);      //close 2nd pipe 
    close(pfds2[1]);
    
    if(bg){
        //if bg processing:update job pid list, print job, continue immediately
        if (num_pipes==2){
            bg_list[num_bg_jobs-1]=pid3;    //if 3 cmds, store cmd 3's pid
        }
        else{
            bg_list[num_bg_jobs-1]=pid2;    //otherwise cmd 2's pid
        }
        printf("[%i] %i\n",num_bg_jobs,bg_list[num_bg_jobs-1]);
   
        }
              
              
    else{   //if no bg, wait for children to finish
        waitpid(pid1,NULL,0);   //wait for first pids
        waitpid(pid2,NULL,0);
        if(num_pipes==2){
            waitpid(pid3,NULL,0);   //wait for 3rd cmd if exits
        }
    }
    num_pipes=0;    //reset num_pipes
   
    

}
