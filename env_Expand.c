#include "shell.h"

void env_Expand(tokenlist *tokens)
{
	int i;
	//iterate through argument tokens
	for (i = 0; i < tokens->size; i++)
	{
		if (tokens->items[i][0] == '$')
		{
			//remove $ char from token and get corresponding env var
			char * buf = (char * ) malloc(sizeof(tokens->items[i])-1);
			strcpy(buf, &tokens->items[i][1]);
			
			
			char * env = getenv(buf);
			
			//GETENV returns NULL on failure
			
			if (env != NULL)
			{
				tokens->items[i]=realloc(tokens->items[i],strlen(env)+1);
				strcpy(tokens->items[i],env);
			}
			
			free(buf);
		}
	}
}