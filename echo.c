void echo(tokenlist *tokens){

    for (int i=1;i<tokens->size;i++){ //iterate through argument tokens
        
        char q;
        q=tokens->items[i][0];
        char r;
        r='$';
        if (q==r){.    //check for env var
        
            //remove $ char from token and get corresponding env var

            char * arg= (char*) malloc(sizeof(tokens->items[i])-1);
            strcpy(arg,&tokens->items[i][1]);
         
            char * env=getenv(arg);
            tokens->items[i]=realloc(tokens->items[i],sizeof(env)+1);
            strcpy(tokens->items[i],env);   

            free(arg); 
         //update token list with expanded token
     
           }      

        }
    
    for (int i=1;i<tokens->size;i++){ 
        printf("%s ",tokens->items[i]);
    }
  }
