void tilda_expand(tokenlist *tokens);

void tilda_expand(tokenlist *tokens){               //expands tokens starting with ~ 
    for (int i=1;i<tokens->size;i++){
        char q;
        q=tokens->items[i][0];
        char r;
        r='~';
        if (q==r){
            char * home=getenv("HOME");                                     //home dir
            char * dir = (char*) malloc(sizeof(tokens->items[i])-1);
            strcpy(dir,&tokens->items[i][1]);                              //dir following ~

           
            tokens->items[i]= (char*) realloc(tokens->items[i],strlen(home)+strlen(dir)+1);  
   
            strcpy(tokens->items[i],home);
 
            strcat(tokens->items[i],dir);

        }
    }
}
