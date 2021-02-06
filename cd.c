#include "shell.h"

void cd (tokenlist * tokens)
{
	int size;
	
	if (tokens->items[1] == NULL)
	{
		size = strlen(getenv("HOME"));
		tokens->items[1] = (char *) realloc(tokens->items[1], size);
		tokens->items[1] = getenv("HOME");
	}
	if (chdir(tokens->items[1])==0)
	{
		char * cwd = getcwd(NULL, 0);
		setenv("PWD", cwd, 1);
		free(cwd);
		return;
		
	}
   printf("cd: No such file or directory: %s", tokens->items[1]);
   return;
}