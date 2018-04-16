#include "cse320_functions.h"

#include <semaphore.h>
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
int addr_count=0;
int files_count;
int N=5;
int reapflag=0;
sem_t mutex;
//char **args;

struct addr_in_use{

	void* addr;
	int ref_count;// = 0;

};


struct files_in_use{

	char* filename;
	int ref_count;// = 0;
	FILE *fptr;
};


void printArray1(struct addr_in_use* ptr);
void printArray2(struct files_in_use* ptr);

struct addr_in_use addr_array[25];
struct files_in_use files_array[25];
/*
   char** inputToArgs(char* command){

// command = run A 1 2 3
char *  p    = strtok (command, " "); // ignore run
p = strtok(NULL, " ");
int n_spaces = 0, i;

while (p) {
args = realloc (args, sizeof (char*) * ++n_spaces);

if (args == NULL){
exit (-1); 
}
args[n_spaces-1] = p;

p = strtok (NULL, " ");
}


args = realloc (args, sizeof (char*) * (n_spaces+1));
args[n_spaces] = 0;

for (i = 0; i < (n_spaces+1); ++i)
printf ("res[%d] = %s\n", i, args[i]);

return args;

}

 */

int main(){
	char* command = (char*)malloc(255);
	char* X= (char*)malloc(255);

	//	srand(time(NULL));
	//	printArray(addr_array);
	//	printArray2(files_array);

	//cse320_free(cse320_malloc(30)); 
	//printArray2(file_array);

	//	cse320_clean();
	//	cse320_fopen("A");
	//	cse320_fopen("A");
	//cse320_fclose("A");
	//	cse320_fclose("B");
	//printArray2(files_array); 

	/*
	   cse320_settimer(1);

	   cse320_fork();
	   printf("-----------------\n");
	   cse320_fork();
	   printf("-----------------\n"); 
	   cse320_fork();
	   printf("-----------------\n"); 
	   sleep(10);
	 */
	//system("ps -eo pid,ppid,stat,cmd");

	//	sem_init(&mutex, 0, 1);


prompt:
	printf("Type your command.\n");

	fgets(command, 100, stdin);
	command[strcspn(command, "\n")] = 0;// REMOVE \n!!!!!

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
		sigfillset(&mask);
		sigprocmask(SIG_SETMASK, &mask, NULL);

		//	sem_wait(&mutex);

		if(pid = fork()==0){
		

			execve(args[0], args, env_args);
			fprintf(stderr, "Oops!\n");


			environ = env_args;
			execvp(args[0], args);
			fprintf(stderr, "Oops again!\n");


		} else { 

			if(waitpid(pid, &status, 0)<0){
				printf("ERROR IN WAITPID\n");
				exit(0);

			}

			sigemptyset(&mask);
			sigprocmask(SIG_SETMASK, &mask, NULL);
			//sem_post(&mutex);		
			//system("ps -eo pid,ppid,stat,cmd");
			//	free(args);
			goto prompt;

		}
	} else if (strstr(command, "help")!=NULL){

printf("run X	-- will run executable X. X can be application name, full path to application, or relative path to application.\n");
printf("quit	-- will exit the program.\n");

		goto prompt;  
	}else if (strstr(command, "exit")!=NULL){
		free(command);
		free(X);
		exit(0);
	} else {
		printf("Unknown Command.\n");

		goto prompt;
	}
}













void *cse320_malloc(size_t size){
	printf("malloc called\n");


	if(addr_count<25){

		void *addr = malloc(size);
		struct addr_in_use *new_addr = malloc(sizeof(struct addr_in_use)); 

		new_addr->addr = addr;
		new_addr->ref_count++;
		addr_array[addr_count]= *new_addr;
		addr_count++;

		return addr;

	} else {
		printf("Not enough memory\n");
		exit(-1);

	}
}

int cse320_free(void *ptr){
	int k;
	//int found = 0;

	for(k=0; k<25; k++){
		if(addr_array[k].addr!=NULL){
			if(addr_array[k].addr == ptr){
				if(addr_array[k].ref_count==0){
					printf("Free: Double free attempt\n");
					errno=EADDRNOTAVAIL;
					exit(-1);
				} else{// >0
					//printf("freed:!!\n");		
					free(addr_array[k].addr);				
					//free(ptr);					
					addr_array[k].ref_count--;
					break;
				}
			}

		}

	}


	if(k==25){ // not found
		printf("Free: Illegal address\n"); 
		errno=EFAULT;
		exit(-1); 
	}



}


void cse320_clean(){
	int j;

	for(j=0; j<25; j++){
		if(addr_array[j].ref_count>0){
			addr_array[j].addr= NULL;
		}
	}


	for(j=0; j<25; j++){
		if(files_array[j].ref_count>0){
			files_array[j].filename= NULL;
		}
	}

}



FILE *cse320_fopen(char *filename){
	int o;
	FILE *ptr = NULL;


	if(files_count<25){

		//printf("F1: %d\n", fileno(ptr)); 
		for(o=0; o<25;o++){
			if(files_array[o].filename!=NULL){
				if(strcmp(filename,files_array[o].filename) ==0 ){
					files_array[o].ref_count = files_array[o].ref_count+1; 
					return files_array[o].fptr;

				}

			}

		}


		if(o==25){
			struct files_in_use* new_file = malloc(sizeof(struct files_in_use));
			new_file->filename = malloc(sizeof(1000));
			strcpy(new_file->filename, filename);
			new_file->ref_count = new_file->ref_count+1;

			ptr = fopen(filename, "r");

			new_file->fptr = ptr;
			files_array[files_count]=*new_file;
			files_count++;
		}
		return files_array[files_count-1].fptr;


	} else {

		printf("Too many opened files\n");
		exit(-1);

	}


}




void cse320_fclose(char* filename){
	int k;

	for(k=0; k<25; k++){
		if(files_array[k].filename!=NULL){
			if(strcmp(filename,files_array[k].filename) ==0){
				if(files_array[k].ref_count==0){
					printf("Close: Ref count is zero\n");
					errno=EINVAL;
					exit(-1);
				} else {// >0
					printf("SHOULD BE 0: %d\n",close(fileno(files_array[k].fptr)));					
					files_array[k].ref_count--;
					break;
				}
			}

		}



	}


	if(k==25){ // not found
		printf("Close: Illegal filename\n"); 
		errno=ENOENT;
		exit(-1); 
	}

}



void cse320_fork(){
	int status;
	pid_t pid= getpid();

	pid = fork();
	printf(" pid: %d \n", getpid());

	if(pid==0){ // child
		printf("CHILd HERE\n");	
		exit(0);

	} else { // parent

		if(!reapflag){
			signal(SIGALRM, cse320_reap);
			alarm(N); // alarm set
			reapflag=1;
		}

	}


}



void cse320_reap(int signum){
	int status;
	//	printf("reap called\n");


	if(getpid()!=0){// parent

		pid_t pid;
		while((pid = waitpid(-1, &status, 0))>0){
			printf("CHILD %d terminated\n", pid);

		}	
	}
	//system("ps -eo pid,ppid,stat,cmd"); 	
}




void cse320_settimer(int newN) {
	N = newN;
}






void printArray1(struct addr_in_use* ptr){
	int j=0;
	for(j=0; j<25;j++){
		printf("addr :%p, count: %d\n", (ptr+j)->addr, (ptr+j)->ref_count);
	}
}



void printArray2(struct files_in_use* ptr){
	int j=0;
	for(j=0; j<1;j++){

		// printf("F2: %d\n", fileno(files_array[j].fptr));
		// printf("filename :%s, count: %d\n", (ptr+j)->filename, (ptr+j)->ref_count);
		printf("filename :%s, count: %d, F: %d\n", (ptr+j)->filename, (ptr+j)->ref_count, fileno((ptr+j)->fptr));
	}
}
