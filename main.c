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
int files_count=0; // opened files
int N=5;
int reapflag=0;
sem_t mutex;

struct addr_in_use{

	void* addr;
	int ref_count;// = 0;

};


struct files_in_use{

	char* filename;
	int ref_count;// = 0;
	FILE *fptr;
	int closed; // =0
};


void printArray1(struct addr_in_use* ptr);
void printArray2(struct files_in_use* ptr);

struct addr_in_use addr_array[25];
struct files_in_use files_array[5];

static void handler (int bar)
{
	printf("handled\n");
	/*some code here. In your case, nothing*/
}





int main(){
	char* command = (char*)malloc(100* sizeof(char*));
	char* X= (char*)malloc(100* sizeof(char*));
	//printArray1(addr_array);
	printArray2(files_array);
	/*
	   void *p = cse320_malloc(40);

	   for(int j=0; j<23; j++){
	   cse320_malloc(30); 

	   }

	   cse320_free(p);
	   cse320_free(p);
	//	printArray1(addr_array);
	//cse320_free(cse320_malloc(200));
	printArray1(addr_array); 	

	//	cse320_clean();
	//printArray1(addr_array);   
	cse320_malloc(60);
	//	printf("addr2: %p\n",cse320_malloc(60));
	printArray1(addr_array);
	//cse320_malloc(1);

	 *///	cse320_clean();

	cse320_fopen("A");
	//cse320_fopen("A");
	printArray2(files_array); 	
	cse320_fclose("A");
	printArray2(files_array); 
	cse320_clean();

	/*
	   cse320_fopen("RR"); 	

	   printf("gggg\n");
	   cse320_fclose("RR");
	   cse320_fopen("A");
	   cse320_fopen("A");
	   cse320_fclose("A");
	   cse320_fclose("A");
	   printf("kkk\n"); 

	   printArray2(files_array); 


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

	//	printArray2(files_array);  

	/*	
		for(int n=0; n<25; n++){
		cse320_fopen("A");
		}


		for(int n=0; n<25; n++){
		cse320_fclose("A");
		}
	 */

	/*
	   cse320_fopen("B");
	   cse320_fopen("B");

	   cse320_fclose("B");
	   cse320_fclose("B");
	 */
	//	printArray2(files_array);  
	//	printf("Files count: %d\n", files_count); 




	/*
	   printf("00Files count: %d\n", files_count);  
	   cse320_fopen("D");
	   cse320_fopen("E");
	   printArray2(files_array);   
	   printf("Files counti22uuuu2: %d\n", files_count);  	
	   cse320_fopen("F");
	   printArray2(files_array); 
	   printf("Files counti222: %d\n", files_count);  
	   cse320_fopen("G");
	   printf("11Files count: %d\n", files_count); 
	   printArray2(files_array);  
	//cse320_fopen("E");
	printArray2(files_array);   

	printf("Files count: %d\n", files_count); 
	cse320_fopen("H");


	 */






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
		//	sem_wait(&mutex);

		if(pid = fork()==0){
			// Unblock for Child
			sigemptyset(&mask);
			sigprocmask(SIG_SETMASK, &mask, NULL);

			execve(args[0], args, env_args);
			//fprintf(stderr, "Oops!\n");


			environ = env_args;
			execvp(args[0], args);
			fprintf(stderr, "Error in running applcation.\n");


		} else { 
			// block
			sigfillset(&mask);
			sigprocmask(SIG_SETMASK, &mask, NULL);


			if(waitpid(pid, &status, 0)<0){
				printf("ERROR IN WAITPID\n");
				exit(0);

			}


			// Install signal handler to ignore pending signals from children 
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
			for(p = 1; p <=31 ; p++)
			{
				signal(p,SIG_DFL);
			}



			//sem_post(&mutex);		
			//system("ps -eo pid,ppid,stat,cmd");
			//	free(args);
			goto prompt;

		}
	} else if (strcmp(command, "help")==0){

		printf("run X	-- will run executable X. X can be application name, full path to application, or relative path to application.\n");
		printf("quit	-- will exit the program.\n");

		goto prompt;  
	}else if (strcmp(command, "exit")==0){
		int q;
		/*	
			for(q=0; q<25; q++){
			if(addr_array[q].addr!=NULL){
			free(&addr_array[q]); 
			}
			}
*/
			for(q=0; q<5; q++){
			if(files_array[q].filename !=NULL){
			free(files_array[q].filename); 
			}
			}


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
	int k,j;
	//addr_count++; 
	if(addr_count<25){
		for(k=0; k<25; k++){   
			if( addr_array[k].addr==NULL && addr_array[k].ref_count==0 ){ 
				//void *addr = malloc(size);
				//struct addr_in_use *new_addr = malloc(sizeof(struct addr_in_use)); 
				/*				
								new_addr->addr= malloc(size);
								new_addr->ref_count++;
								addr_array[k]= *new_addr;
				 */
				addr_array[k].addr = malloc(size);
				addr_array[k].ref_count++;

				addr_count++;
				return addr_array[k].addr;
			}
		}

		if(k==25){

			for(j=0; j<25; j++){// replace it

				if( addr_array[j].ref_count==0 ){ 
					//	void *addr = malloc(size);			
					addr_array[j].addr =malloc(size);// malloc(sizeof(void*));
					addr_array[j].ref_count++;
					addr_count++;
					return addr_array[j].addr;
				}






			}

		}
	} else {
		printf("Not enough memory\n");
		//addr_count--;	
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
					free(addr_array[k].addr);				
					//	addr_array[k].addr=NULL; NONONO					
					addr_array[k].ref_count--;
					addr_count--;			
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
		if( addr_array[j].addr !=NULL){
			//printf("once\n"); 
			if(addr_array[j].ref_count>0){
				free(addr_array[j].addr);
				//printf("never\n"); 
				addr_array[j].ref_count--;	
			}
		}
	}

	for(j=0; j<5; j++){
		if( files_array[j].filename!= NULL && files_array[j].ref_count>0 ){
			printf("nono\n");		
			close(fileno(files_array[j].fptr));
			//free(files_array[j].filename);
			free(files_array[j].fptr);			
			files_array[j].ref_count=0;
		}
	}

}



FILE *cse320_fopen(char *filename){
	int o,l,m;
	FILE *ptr = NULL;
	files_count++;
	printf("opend\n");



	// Already exist in files_array
	for(o=0; o<5;o++){
		if(files_array[o].filename!=NULL){
			if(strcmp(filename,files_array[o].filename) ==0 ){
				files_array[o].ref_count++;					
				files_count--;			
				return files_array[o].fptr;

			}

		}

	}

	if(files_count>5) {
		printf("Too many opened files\n");
		files_count--;	
		exit(-1);

	}	
	// not in the array, opening for the first time, empty spot


	if(o==5){
		for(l=0; l<5; l++){

			if(files_array[l].filename ==NULL && files_array[l].ref_count==0){
				files_array[l].filename = malloc(100*sizeof(char*));
				strcpy(files_array[l].filename, filename);
				printf("TTHISSS\n");
				files_array[l].ref_count++;
				files_array[l].fptr = malloc(sizeof(FILE*));
				files_array[l].fptr = fopen(filename, "r");
				return files_array[l].fptr;
			}

		}


		if(l==5){// array is full, replace ref_count =0 spot

			for(m=0; m<5; m++){

				if( files_array[m].ref_count==0 ){
					strcpy(files_array[m].filename, filename);
					files_array[m].ref_count ++;
					files_array[m].fptr=fopen(filename, "r");

					return files_array[m].fptr;
				}

			}



		}	
	}


	/*
	   } else {

	   printf("Too many opened files\n");
//files_count--;	
exit(-1);

}

	 */
}




void cse320_fclose(char* filename){
	int k;

	for(k=0; k<5; k++){
		if(files_array[k].filename!=NULL){
			if(strcmp(filename,files_array[k].filename) ==0){
				if(files_array[k].ref_count==0){
					printf("Close: Ref count is zero\n");
					errno=EINVAL;
					exit(-1);
				} else {// >0
					printf("HH\n");
					files_array[k].ref_count--;

					if(files_array[k].ref_count==0){
						printf("h1h1h1\n");					
						files_count--; // file closed completely

						printf("SHOULD BE 0: %d\n",close(fileno(files_array[k].fptr)));					
					//	free(files_array[k].filename);  
						free(files_array[k].fptr);			
						//	break;
					}	
					break;			
				}
			}

			//break;
		}
	}



	if(k==5){ // not found
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
		printf("At %d addr :%p, count: %d\n",j+1, (ptr+j)->addr, (ptr+j)->ref_count);
	}
}



void printArray2(struct files_in_use* ptr){
	int j=0;

	for(j=0; j<5;j++){
		//	printf("At %d   count: %d\n  F: %d\n",j+1, (ptr+j)->ref_count,fileno((ptr+j)->fptr) );
		printf("At %d filename :%s, count: %d, FP: %p ",j+1, (ptr+j)->filename, (ptr+j)->ref_count, (ptr+j)->fptr);
		if((ptr+j)->fptr!=NULL){
			printf("FD: %d\n", fileno((ptr+j)->fptr));

		}
		else {
			printf("FD:  \n"); 


		}
	}

}
