//is_path seems to work to find ls path but seg faults when trying to exit afterwards 
//included the main with calls to the exit funcs in exit.c


int main(int argc, char const *argv[])
{
    size_t size = 10;
    char * buffer;
    
    while (1)

    {   
        time(&beginning);
        
        PrintPrompt();
        buffer=get_input();

        tokenlist *tokens = get_tokens(buffer);

        tilda_expand(tokens);
        env_expand(tokens);

        time(&end); 
        totalruntime+=difftime(end,beginning);
 
        time(&beginning);

        bool p=is_Path(tokens->items[0]);

        //get_command(tokens);
     
        time(&end); 
                                                                    //ends timing the shell
        time_shell();      
                                                             //calculates runtime and adds to total and compares to longest running command
        if(exitshell(tokens)==true) 
        {
            waitpid(-1,NULL,0);
            break;
        }

        
        free_tokens(tokens);
    }
    

    free(buffer);
    
    return 0;
}

bool is_Path(char * input)
{
    //char ** path_list;
    char * path = getenv("PATH");

    char * pathcopy = (char *)malloc(strlen(path)+1);
    strcpy(pathcopy,path);

    char * token = strtok(pathcopy,":");

    char * fpath= (char *) malloc(strlen(token)+strlen(input)+2);;

    while (token != NULL)
    {   
       
        strcpy(fpath,token);
        strcat(fpath,"/");
        strcat(fpath,input);

        if( access(fpath, X_OK)!=0){
        //printf( " %s\n", token ); //printing each token
            token = strtok(NULL, ":");
            fpath=(char*) realloc(fpath,strlen(token)+strlen(input)+2); 
            continue;
        }
        printf("cmd path found\n");
        printf("%s",fpath);
        //execute command
        free(fpath);
        free(pathcopy);
        return true;

    }

    printf("Bash: command not found: %s\n", input);
    free(fpath);
    free(pathcopy);
    return false;
}
