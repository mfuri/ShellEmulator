//Does not currently account for ".." 
//updated to show full path after change and handle different input formats

    
void cd(tokenlist *tokens){
    char * path;
    char * pwd;
    char * home;
 
    pwd=getenv("PWD");
    home=getenv("HOME");
    
    if (tokens->size==1){                               //If no argument, cd to home
        path= (char*)malloc(strlen(home)+1);
        strcpy(path,home);            
    }
    else {                                              //otherwise get arg
        path= (char *) malloc(strlen(tokens->items[1])+1);
        strcpy(path, tokens->items[1]);
              
        if (strstr(path,home)==NULL){                   //if not a complete path, add home to start of path
            path = (char*) realloc(path, sizeof(path)+strlen(home)+1);
            strcpy(path,home);
            strcat(path,"/");
            strcat(path,tokens->items[1]);
                 
        }
        if (access(path,X_OK)!=0){
            printf("\n%s\n", "Directory not found");
             printf("\n%s\n", path);
            return;                                 //print error and exit if path not an accesible directory
        }
    }
    strcpy(pwd,path);           //change cwd
    free(path);  
    return;
}
