#include "shell.h"

void tilde_Expand(tokenlist *tokens)
{             
	int i;
	for (i = 0; i < tokens->size; i++)
	{
		//Check to see if first character is ~
		//Expands using wordexp
		if (tokens->items[i][0] == '~')
		{
			wordexp_t p;
			int size;
			wordexp(tokens->items[i], &p, 0);
			size = strlen(p.we_wordv[0]);
			
			tokens->items[i] = (char *) realloc(tokens->items[i], size);
			
			strcpy(tokens->items[i], p.we_wordv[0]);
			wordfree(&p);
		}
	}
}
