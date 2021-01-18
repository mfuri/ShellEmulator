//Does not currently account for ".." 
//if changing to a subdirectory of pwd, new prompt pwd shows just subdirectory rather than whole path 
    
void cd(tokenlist *tokens){  
    char * path;
    char * pwd;
    pwd=getenv("PWD");

    if (tokens->size==1){
        path=getenv("HOME");
        
             
    }
    else {
        path= (char *) malloc(strlen(tokens->items[1])+1);
        strcpy(path, tokens->items[1]);
        if (access(path,X_OK)!=0){
            printf("\n%s\n", "Directory not found");
            return;
        }

    }
    strcpy(pwd,path);
   

    return;
   
    }
