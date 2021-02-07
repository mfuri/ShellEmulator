bool get_command(tokenlist *tokens){
    if (strcmp(tokens->items[0],"cd")==0){
        cd(tokens->items[1]);
        return true;
    }

    if (strcmp(tokens->items[0],"echo")==0){
        echo(tokens);
        return true;
    }

   
    if (strcmp(tokens->items[0],"exit")==0){
    //    ex();
        return true;   
    }

    if (strcmp(tokens->items[0],"jobs")==0){
        jobs(tokens);
        return true;

    }
    //printf("not a built in command");
    return false;
}
