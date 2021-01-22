#include <time.h>

time_t beginning, end;                                              //global variables
int currentruntime, totalruntime, longestruntime;	

void time_shell();                                                  //functions declerations
bool exitshell(tokenlist *tokens);   


//This goes in the main while loop
                                                               
	time(&beginning);														//begins timing the shell 

    //prompt and getting input and tokens//


    	time(&end);																//ends timing the shell
	time_shell();                                                           //calculates runtime and adds to total and compares to longest running command
	if(exitshell(tokens)==true) 
	{
		waitpid(-1,NULL,0);
    		break;
	};

    //freeing input and tokens



//function definitions
bool exitshell(tokenlist *tokens)                                  
	char *c = "exit";
	if(strcmp(c,tokens->items[0])==0)
	{
		printf("Shell ran for %d seconds and took %d seconds to execute one command.", totalruntime, longestruntime);
		return true;
	}
	else
	{
		return false;
	}
	
}

void time_shell()
{
	currentruntime = difftime(end,beginning);								//store command execution run time.
	totalruntime  += currentruntime;										//time storing
	if(longestruntime<currentruntime)
	{
		longestruntime = currentruntime;
	}

}
