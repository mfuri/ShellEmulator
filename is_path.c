bool is_Path(tokenlist * tokens)
{
int i;
//for (i = 0; i < strlen(tokens->items[0]); i ++)

char * path = getenv("PATH");

char * pathcopy = (char *)malloc(strlen(path)+1);
strcpy(pathcopy,path);

char * token = strtok(pathcopy,":");

char * fpath= (char *) malloc(strlen(token)+strlen(input)+2);

while (token != NULL)
	{
		fpath=(char*) realloc(fpath,strlen(token)+strlen(input)+2);
		
		strcpy(fpath,token);
		strcat(fpath,"/");
		strcat(fpath,input);
		
		if( access(fpath, X_OK)!=0){
			//printf( " %s\n", token ); //printing each token
			token = strtok(NULL, ":");
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
return false;]