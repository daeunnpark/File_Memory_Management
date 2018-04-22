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
#include <stddef.h> 
#include <pthread.h>
#include <sys/mman.h>
#include <time.h>


int addr_count=0; // number of addr
int files_count=0; // number of opened files

struct addr_in_use addr_array[25];
struct files_in_use files_array[25];

// for reaping children with SIGALRM  
int reapflag1=0;
int count1 = 0;
int N1=5; // time interval to call alarm reaping function

// for reaping chilren with Thread   
int reapflag2=0;
int count2=0; // for reaping chilren with Thread
int N2=5;  // time interval to call reaping thread
int pidList[10]; // array of int to store pid of chidlren


void* cse320_set(struct addr_in_use* addr_array2, struct files_in_use* files_array2){

	sem_t mutex;
	sem_init(&mutex, 0, 1);
	sem_wait(&mutex);

	addr_array[0] = addr_array2[0];
	files_array[0] = files_array2[0];

	sem_post(&mutex);


}
void * cse320_setPidList(int* pidList2 ){

	pidList[0]= pidList2[0];

}


void *cse320_malloc (size_t size){ 
	sem_t mutex;
	sem_init(&mutex, 0, 1);
	sem_wait(&mutex);

	int k,j;
	if(addr_count<25){
		for(k=0; k<25; k++){   
			if( addr_array[k].addr==NULL && addr_array[k].ref_count==0 ){ 
				addr_array[k].addr = malloc(size);
				addr_array[k].ref_count++;

				addr_count++;
				return addr_array[k].addr;
			}
		}

		if(k==25){

			for(j=0; j<25; j++){// replace it

				if( addr_array[j].ref_count==0 ){ 
					addr_array[j].addr =malloc(size);
					addr_array[j].ref_count++;
					addr_count++;
					return addr_array[j].addr;
				}

			}

		}

	} else {

		printf("Not enough memory\n");

		cse320_clean();
		sem_post(&mutex); 
		exit(-1);

	}
	sem_post(&mutex);
}




void cse320_free(void *ptr){
	sem_t mutex;
	sem_init(&mutex, 0, 1);
	sem_wait(&mutex);

	int k;

	for(k=0; k<25; k++){
		if(addr_array[k].addr!=NULL){
			if(addr_array[k].addr == ptr){
				if(addr_array[k].ref_count==0){
					printf("Free: Double free attempt\n");

					cse320_clean();

					errno=EADDRNOTAVAIL;
					sem_post(&mutex); 
					exit(-1);
				} else {// >0
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

		cse320_clean();

		errno=EFAULT;
		sem_post(&mutex); 		
		exit(-1); 
	}


	sem_post(&mutex); 
}




FILE *cse320_fopen(char *filename){

	sem_t mutex;
	sem_init(&mutex, 0, 1);
	sem_wait(&mutex);


	int o,l,m;
	files_count++;

	(struct files_in_use*)files_array;
	// Already exist in files_array
	for(o=0; o<25;o++){
		if(files_array[o].filename!=NULL){
			if(strcmp(filename,files_array[o].filename) ==0 ){
				files_array[o].ref_count++;					

				files_count--;
				return files_array[o].fptr;
			}

		}

	}

	if(files_count>25) {
		printf("Too many opened files\n");

		cse320_clean();

		files_count--;	
		sem_post(&mutex); 		
		exit(-1);

	}	



	// not in the array, opening for the first time, at empty spot
	if(o==25){
		for(l=0; l<25; l++){

			if(files_array[l].filename ==NULL && files_array[l].ref_count==0){
				files_array[l].filename = malloc(100*sizeof(char*));
				strcpy(files_array[l].filename, filename);
				files_array[l].ref_count++;

				FILE* fp = fopen(filename, "w");
				files_array[l].fptr = fdopen (dup (fileno (fp)), "w");  			
				fclose(fp);
				return files_array[l].fptr;			
			}

		}



		// files_array is full, replace first spot with ref_count =0  
		if(l==25){
			for(m=0; m<25; m++){

				if( files_array[m].ref_count==0 ){
					strcpy(files_array[m].filename, filename);
					files_array[m].ref_count ++;

					FILE* fp = fopen(filename, "w");				
					files_array[m].fptr = fdopen (dup (fileno (fp)), "w");  			
					fclose(fp);
					return files_array[m].fptr; 
				}

			}



		}	
	}
	sem_post(&mutex); 

}



void cse320_fclose(char* filename){
	sem_t mutex;
	sem_init(&mutex, 0, 1);
	sem_wait(&mutex);

	int k, flag;
	flag= 0;
	for(k=0; k<25; k++){
		if(files_array[k].filename!=NULL){
			if(strcmp(filename,files_array[k].filename) ==0){
				if(files_array[k].ref_count==0){
					printf("Close: Ref count is zero\n");

					cse320_clean();

					errno=EINVAL;
					sem_post(&mutex); 		
					exit(-1);
				} else {// >0
					files_array[k].ref_count--;

					if(files_array[k].ref_count==0){
						// close file completely 						
						files_count--;
						close(fileno(files_array[k].fptr));	
						fclose(files_array[k].fptr);
						//printf("SHOULD BE 0: %d\n",close(fileno(files_array[k].fptr)));	
						//printf("SHOULD BE -1:  %d\n",fclose(files_array[k].fptr));
					}
					return;
				}
			}

		}
	}

	if( k==25){ // not found
		printf("Close: Illegal filename\n"); 

		cse320_clean();

		errno=ENOENT;
		sem_post(&mutex); 
		exit(-1); 
	}

	sem_post(&mutex); 
}






void cse320_clean(){

	sem_t mutex;
	sem_init(&mutex, 0, 1);
	sem_wait(&mutex);


	int j;

	for(j=0; j<25; j++){
		if( addr_array[j].addr !=NULL){
			if(addr_array[j].ref_count>0){
				free(addr_array[j].addr);
				addr_array[j].ref_count--;	
			}
		}
	}





	for(j=0; j<25; j++){
		if( files_array[j].filename!= NULL){
			if( files_array[j].ref_count>0 ){
				close(fileno(files_array[j].fptr));
				fclose(files_array[j].fptr);

				//printf("2 SHOULD BE 0: %d\n",close(fileno(files_array[j].fptr)));	
				//printf("2 SHOULD BE -1: %d\n",fclose((files_array[j].fptr)));	


				files_array[j].ref_count=0;
			}
			// for all
			free(files_array[j].filename);
		}	

	}
	sem_post(&mutex); 
}





void cse320_fork(){
	/*
	   sem_t mutex;
	   sem_init(&mutex, 0, 1);
	   sem_wait(&mutex);

	 */

	int status;
	pid_t pid= getpid();

	pid = fork();
	//	printf(" pid: %d\n ", getpid());

	if(pid==0){ // child
		exit(0);

	} else { // parent
		count1++;
		if(!reapflag1){
			signal(SIGALRM, cse320_reap);

			reapflag1=1;
			alarm(N1); // alarm set

		}
	}

}

void cse320_reap(int signum){
	int status;
	//printf("reap called\n");


	if(getpid()!=0){// parent
		pid_t pid;
		while((pid = waitpid(-1, &status, 0))>0){
			//	printf("CHILD %d terminated\n", pid);
			count1--;
			//	printf("in child count: %d\n", count);  
		}	
	}

	if(count1>0){
		alarm(N1);
		sleep(N1);
	} else { // exit reaping
		reapflag1= 0;
		//system("ps -eo pid,ppid,stat,cmd"); 	
	}
}




void cse320_fork_thread(){
	int status;
	pthread_t tid1;

	pid_t pid= getpid();
	pid = fork();


	if(pid==0){ // child
		exit(0);

	} else { // parent
		int j;

		// add at first empty spot in pidList
		for(j=0; j<10; j++){ 
			if(pidList[j]==0){  
				pidList[j] = pid;  
				count2++; 
				break;
			}
		}

		// create thread for only once
		if(reapflag2==0){
			reapflag2=1;
			pthread_create(&tid1, NULL, cse320_reap_thread, &pidList);  
			//printf("the main thread continues with its execution\n"); 
			//	printf("the main thread finished\n"); 

		}

	}

}





void *cse320_reap_thread(void *pidList){

	int status;

	while(1){
		sleep(N2);
		if(getpid()!=0){// parent

			pid_t pid2;
			int *pidlist = pidList;
			/*
			   for(int k=0; k<10; k++){
			   printf("pid at %d : %d\n",k+1, pidlist[k]);
			   }	
			 */
			while((pid2 = waitpid(-1, &status, 0))>0){
				//	printf("Thread CHILD %d terminated\n", pid2);
				int o;	
				for(o = 0; o<10; o++){

					// remove from list
					if(pidlist[o]==pid2){
						pidlist[o]=0;
						count2--;
						break;
					}
				}


			}

			if(count2==0){
				//	printf("detached\n");
				pthread_detach(pthread_self());	
				reapflag2 = 0;	
				//system("ps -eo pid,ppid,stat,cmd");  		
				break;
			}
		}

	}

}



void cse320_settimer1(int newN) {
	sem_t mutex;
	sem_init(&mutex, 0, 1);
	sem_wait(&mutex);

	N1 = newN;
	sem_post(&mutex); 
}

int cse320_gettimer1(){
	sem_t mutex;
	sem_init(&mutex, 0, 1);
	sem_wait(&mutex);

	return N1;
	sem_post(&mutex); 
}

void cse320_settimer2(int newN) {
	sem_t mutex;
	sem_init(&mutex, 0, 1);
	sem_wait(&mutex);

	N2 = newN;
	sem_post(&mutex); 
}

int cse320_gettimer2(){
	sem_t mutex;
	sem_init(&mutex, 0, 1);
	sem_wait(&mutex);

	return N2;
	sem_post(&mutex); 
}

