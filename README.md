# COP4610-Proj1


# List of files:
-	parser.c
	-	Main while loop provided for us. Calls external functions from different
		files as well as implements background processing.
-	cd.c
	-	Implements the cd functionality of the shell.
-	echo.c
	-	Prints to the screen from the input provided.
-	env_Expand.c
	-	Expands all environmental variables found within a token list.
-	is_Path.c
	-	Determines whether a command is located in $PATH.
	piping.c
		Handles pipe execution.
-	print_Prompt.c
	-	Prints the user's name, 
-	provided_Functions.c
	-	Functions for token creation and manipulation provided for us.
-	redirection.c
	-	Handles file I/O, called from main when necessary.
-	shell.h
	-	All function declarations and included libraries are located in this header file.
-	tilde_Expand.c
	-	Expands all tildes when appropriate for every token in the tokenlist.
-	makefile
	-	Used to compile program, outputs "shell" executable.
-	Git Screenshots
	-	Directory with screenshots of all commits.
-	gitcommitlog.md
	-	Log files from "git log" command.

Readme.md and gitcommitlog.md are included in the

# Team Contributions
-	Michael Tafuri contributed to:
	-	Echo()
	-	cd()
	-	is_Path()
	-	tilde_Expand()
	-	print_Prompt()
	-	env_Expand()
	-	shell.h

-	Teammate 1 contributed to:
	- 	checks_and_exit()
	- 	redirect_tokens()
	- 	openfd()
	- 	closefd()
	-	time_command()
	-	external_cmd()
	- 	shell.h
	
-	Teammate 2 contributed to:
	-	echo_Function()
	- 	cd()
	- 	is_Path()
	-	check_and_exit()
	-	external_cmd()
	-	get_command()
	-	piping.c (all functions)
	-	redirection.c (all functions)
	-	check_background()
	-	run_background()
	-	update_jobs()
	-	exitshell()
	-	time_command()
	- 	jobs()

# How to compile with Makefile
-	To compile, the command is "make shell". This will create object files as well
	as link files to create the "shell" executable.
	
-	To remove all created object files, run "make clean".

# Bugs
-	There appear to be some memory leaks in the implementation of piping with background processing. Sometimes these commands may not run properly. We attempted debugging using valgrind and several different implementations of memory allocation. It appears the errors originate in pipe_exec()'s call to update_jobs(). The bug first appeared today as we were finalizing our code organization. It appears somewhat inconsistently. 

# Extra Credit
-	Shellception: Shell can execute from within itself.
