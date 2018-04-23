#include <unistd.h>
#include "errno.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>

extern char **environ; 


// dummy handler to ignore signals 
static void handler (int bar){     
	// nothing
}

int main(){

	char* command = (char*)malloc(100* sizeof(char*));
	char* X= (char*)malloc(100* sizeof(char*));


prompt:
	printf("Type your command.\n");

	fgets(command, 100, stdin);
	// REMOVE \n!!!!! 
	command[strcspn(command, "\n")] = 0;

	if(strstr(command, "run")!=NULL){
		int i = 0;
		char *p = strtok (command, " ");
		p = strtok (NULL, " ");  
		char *args[6];

		while (p != NULL) {
			args[i++] = p;			
			p = strtok (NULL, " ");
		}
		args[i]=NULL;


		//		char* args[] = {"ls", NULL};
		char *env_args[] = { "PATH=/bin", "USER=me", NULL }; 
		pid_t pid= getpid();
		int status;
		sigset_t mask;
		// Block any Signal	
		sigfillset(&mask);
		sigprocmask(SIG_SETMASK, &mask, NULL);

		if(pid = fork()==0){
			// Unblock for Child
			sigemptyset(&mask);
			sigprocmask(SIG_SETMASK, &mask, NULL);

			execve(args[0], args, env_args);
			//fprintf(stderr, "Oops!\n");

			environ = env_args;
			execvp(args[0], args);
			//fprintf(stderr, "Error in running applcation.\n");

			exit(0);


		} else { 
			// block
			sigfillset(&mask);
			sigprocmask(SIG_SETMASK, &mask, NULL);


			if(waitpid(pid, &status, 0)<0){
				printf("ERROR IN WAITPID\n");

				free(command);
				free(X);

				exit(0);

			}


			// Install dummy signal handler to ignore pending signals from children 
			int k;
			for(k = 1; k <=31 ; k++)
			{
				signal(k,handler);
			}

			// unblock
			sigemptyset(&mask);
			sigprocmask(SIG_SETMASK, &mask, NULL);

			// reset handler for parents
			int p;
			for(p = 1; p <=31 ; p++){
				signal(p,SIG_DFL);
			}

			//system("ps -eo pid,ppid,stat,cmd");
			goto prompt;

		}
	} else if (strcmp(command, "help")==0){

		printf("run X	-- will run executable X. X can be application name, full path to application, or relative path to application.\n");
		printf("quit	-- will exit the program.\n");

		goto prompt;  
	} else if (strcmp(command, "exit")==0){
		free(command);
		free(X);

		exit(0);
	} else {
		printf("Unknown Command.\n");

		goto prompt;
	}
}


