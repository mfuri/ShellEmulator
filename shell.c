#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void PrintPrompt();

int main(int argc, char const *argv[])
{
	size_t size = 10;
	char * buffer;
	do
	{
		PrintPrompt();

		buffer = (char *) malloc (size);

		getline (&buffer, &size, stdin);
	}
	while (strcmp(buffer, "exit") != 0);
	return 0;
}

void PrintPrompt()
{
	int host;
	char hostname[256];
	char * user = getenv("USER");
	char * pwd = getenv("PWD");
	host = gethostname(hostname, sizeof(hostname));
	
		
	if (host == -1)
		printf("error getting hostname\n");

	printf("%s@%s : %s > ",user,hostname,pwd);
}
