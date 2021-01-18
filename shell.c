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
		buffer=get_input();
		//buffer = (char *) malloc (size);
		//getline (&buffer, &size, stdin);
		
		
		//below is commented out since we don't have header files yet
		
		//tokenlist *tokens = get_tokens(buffer);  
		
		//tilda_expand(tokens);
		//env_expand(tokens);

	

        	//fnptr cmd = get_command(tokens);
        	//cmd(tokens);
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
