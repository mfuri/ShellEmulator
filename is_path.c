#include "shell.h"

bool is_Path(tokenlist * tokens)
{
	/*int i;
	for (int i = 0; i < strlen(tokens->items[0]); i++)
	{
		if (strchr(tokens->items[0],'/') != NULL)
			return true;
	}*/
	
	char * path = getenv("PATH");

	char * pathcopy = (char *)malloc(strlen(path)+1);
	strcpy(pathcopy,path);

	char * token = strtok(pathcopy,":");

	char * fpath = (char *) malloc(strlen(token)+strlen(tokens->items[0])+2);

	while (token != NULL)
	{
			fpath = (char*) realloc(fpath,strlen(token)+strlen(tokens->items[0])+2);
			strcpy(fpath,token);
			strcat(fpath,"/");
			strcat(fpath,tokens->items[0]);
		
		if (access(fpath, X_OK)!= 0)
		{
			token = strtok(NULL, ":");
			continue;
		}
		tokens->items[0] = (char*) realloc(tokens->items[0], strlen(fpath)+1);
		strcpy(tokens->items[0], fpath);
		
		free(fpath);
		free(pathcopy);
		return true;
	}
	
	free(fpath);
	free(pathcopy);
	return false;
}
