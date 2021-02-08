#include "shell.h"

void Echo(tokenlist * tokens)
{
	int i;
	for (i = 1; i < tokens->size - 1; i++)
	{
		printf("%s ", tokens->items[i]);
	}
	printf("%s\n", tokens->items[tokens->size - 1]);
}
