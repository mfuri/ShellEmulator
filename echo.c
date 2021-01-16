void echo(tokenlist *tokens){

    for (int i=1;i<tokens->size;i++){ //iterate through argument tokens
        
        int chk; 
        char q;
        q=tokens->items[i][0];
        char r;
        r='$';
        if (q==r){
  
            char * arg= (char*) malloc(sizeof(tokens->items[i])-1);
            arg=&tokens->items[i][1];
           char * env=getenv(arg);
           tokens->items[i]=env;   
           }      

        }
    
    for (int i=0;i<tokens->size;i++){
        printf("%s ",tokens->items[i]);
    }
  }
