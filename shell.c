#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
	size_t size = 10;
	char * buffer;
	do
	{
		int host;
		char hostname[256];
		char * user = getenv("USER");
		host = gethostname(hostname, sizeof(hostname));
		
		if (host == -1)
			printf("error getting hostname\n");

		printf("%s@[%s] > ",user,hostname);

		buffer = (char *) malloc (size);

		getline (&buffer, &size, stdin);
	}
	while (strcmp(buffer, "exit") != 0);
	return 0;
}
