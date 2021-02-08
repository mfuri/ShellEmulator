
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

void pipe_exec()
{
//this works for one pipe but not 2. I tried added 3rd command implemntation not sure whats going wrong. 
//since theirs multiple forks im planning to just have this call is_Path and then skip the calls to is_Path and external cmd
//neads to add flag for bg processing

    tokenlist * cmd1=get_tokens(CMDS[0]);         //gets command tokens ist from list of string cmd lines
    tokenlist * cmd2=get_tokens(CMDS[1]);       
    tokenlist * cmd3;
    
    if (!is_Path(cmd1) || !is_Path(cmd2)){      //check paths
        printf("command not found");
        return;
    }
    if (num_pipes==2){
        cmd3=get_tokens(CMDS[2]);
        if(!is_Path(cmd3)){
            printf("command not found");
            return; 
        }
    }
    
    int p_fds1[2];  //this one works for |
    int p_fds2[2];  //trying to use this for ||
    
    pipe(p_fds1);

    pid_t pid1=fork();
    if (pid1==0){
        printf("in child 1\n");
        close(p_fds1[0]); //close unused end of pipe
        close(1);
        dup(p_fds1[1]);    //redirect output & cmd 1
        execv(cmd1->items[0],cmd1->items);
        exit(1);
    }

    pid_t pid2=fork();
    if (pid2==0){
        printf("in child 2\n");

        //redirect input & cmd 2
        close(0);
        dup(p_fds1[0]);

        if (num_pipes==2){ //redirect output for cmd 3
           
            close(1);
            dup(p_fds1[1]);
        }
        else{
            close(p_fds1[1]);
        }
        execv(cmd2->items[0],cmd2->items);

        exit(1);
    }
    
    if (num_pipes==2){        //not working
        p_fds2[0]=p_fds1[1];
        pipe(p_fds2);
        pid_t pid3 = fork();
        if (pid3==0){
            printf("in child 3\n");
            
            close(p_fds2[1]);
            close(0);
            dup(p_fds2[0]);

            execv(cmd3->items[0],cmd3->items);
            exit(1);
        }
        close(p_fds2[0]);
        close(p_fds2[1]);
        waitpid(pid3,NULL,0);

        
    }
    
    close(p_fds1[0]);
    close(p_fds1[1]);


    waitpid(pid1,NULL,0);
    waitpid(pid2,NULL,0);
    num_pipes=0;
}
