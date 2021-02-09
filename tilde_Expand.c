#include "shell.h"

void tilde_Expand(tokenlist *tokens)
{
	int i;
	for (i = 0; i < tokens->size; i++)
	{
		//Check to see if first character is ~
		//Expands using wordexp
		if (tokens->items[i][0] == '~' && tokens->items[i] != NULL)
			{
				/*tokens->items[i]++;
				char * home = getenv("HOME");
				char * buf = (char *) malloc(strlen(tokens->items[i])+strlen(home));
				
				strcpy(buf, home);
				strcat(buf, tokens->items[i]);
				
				tokens->items[i] = (char *) realloc(tokens->items[i], strlen(tokens->items[i])+strlen(home));
				
				strcpy(tokens->items[i], buf);
				
				free(buf);*/
				
				wordexp_t p;
				int size;
				wordexp(tokens->items[i], &p, 0);
				
				tokens->items[i] = (char *) realloc(tokens->items[i], strlen(tokens->items[i]) + strlen(p.we_wordv[0]));
				
				strcpy(tokens->items[i], p.we_wordv[0]);
				wordfree(&p);
			}
	}
	return;
}
