#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	char * input;
	do
	{
		scanf("%s\n", input);
	}
	while (strcmp(input, "exit") != 0);
	return 0;
}
