#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	size_t size = 10;
	char * buffer;
	do
	{
		char * user = getenv("USER");

		printf("%s",user);

		buffer = (char *) malloc (size);

		getline (&buffer, &size, stdin);
	}
	while (strcmp(buffer, "exit") != 0);
	return 0;
}
