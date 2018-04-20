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
#include "cse320_functions.h"
#include <stddef.h> 

// .c


int addr_count=0;
int files_count=0; // opened files
int N=5;
int reapflag=0;
sem_t mutex;

struct addr_in_use addr_array[25];
struct files_in_use files_array[5];


void printArray1(void* ptr);
void printArray2(void* ptr);

void reset(struct addr_in_use* addr_array2, struct files_in_use* files_array2){

	addr_array[0] = addr_array2[0];
	files_array[0] = files_array2[0];

}

void *cse320_malloc ( size_t size){ 

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

		exit(-1);

	}
}




void cse320_free(void *ptr){
	int k;

	for(k=0; k<25; k++){
		if(addr_array[k].addr!=NULL){
			if(addr_array[k].addr == ptr){
				if(addr_array[k].ref_count==0){
					printf("Free: Double free attempt\n");

					cse320_clean();

					errno=EADDRNOTAVAIL;

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
		exit(-1); 
	}



}




FILE *cse320_fopen(char *filename){
	int o,l,m;
	files_count++;

	(struct files_in_use*)files_array;
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

		cse320_clean();

		files_count--;	
		exit(-1);

	}	



	// not in the array, opening for the first time, at empty spot
	if(o==5){
		for(l=0; l<5; l++){

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
		if(l==5){
			for(m=0; m<5; m++){

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


}



void cse320_fclose(char* filename){

	int k;
	for(k=0; k<5; k++){
		if(files_array[k].filename!=NULL){
			if(strcmp(filename,files_array[k].filename) ==0){
				if(files_array[k].ref_count==0){
					printf("Close: Ref count is zero\n");

					cse320_clean();

					errno=EINVAL;
					exit(-1);
				} else {// >0
					files_array[k].ref_count--;

					if(files_array[k].ref_count==0){
						// close file completely 						
						files_count--;
						//close(fileno(files_array[k].fptr));	
						//fclose(files_array[k].fptr);
						printf("SHOULD BE 0: %d\n",close(fileno(files_array[k].fptr)));	
						printf("SHOULD BE -1:  %d\n",fclose(files_array[k].fptr));
					}	
					break;			
				}
			}

		}
	}



	if(k==5){ // not found
		printf("Close: Illegal filename\n"); 

		cse320_clean();

		errno=ENOENT;

		exit(-1); 
	}

}






void cse320_clean(){
	int j;

	for(j=0; j<25; j++){
		if( addr_array[j].addr !=NULL){
			if(addr_array[j].ref_count>0){
				free(addr_array[j].addr);
				addr_array[j].ref_count--;	
			}
		}
	}





	for(j=0; j<5; j++){
		if( files_array[j].filename!= NULL){
			if( files_array[j].ref_count>0 ){
				printf("nono\n");		
				//	close(fileno(files_array[j].fptr));			
				//	fclose(files_array[j].fptr);

				printf("2 SHOULD BE 0: %d\n",close(fileno(files_array[j].fptr)));	
				printf("2 SHOULD BE -1: %d\n",fclose((files_array[j].fptr)));	


				files_array[j].ref_count=0;
			}
			// for all
			free(files_array[j].filename);
			//free(files_array[j].fptr);  		

		}	
	}
}





void cse320_fork(){
	int status;
	pid_t pid= getpid();

	pid = fork();
	printf(" pid: %d\n ", getpid());




	if(pid==0){ // child
		printf("CHILd HERE\n");	
		exit(0);

	} else { // parent
		printf("gg111111\n"); 
		if(!reapflag){
			printf("gg\n");

			signal(SIGALRM, cse320_reap);

			reapflag=1;
			alarm(N); // alarm set

			//	sleep(N);		

		}

	}


}



void cse320_reap(int signum){
	int status;
	printf("reap called\n");


	if(getpid()!=0){// parent
		pid_t pid;
		while((pid = waitpid(-1, &status, 0))>0){
			printf("CHILD %d terminated\n", pid);
		}	
	}
	alarm(N);
	//	system("ps -eo pid,ppid,stat,cmd"); 	
	sleep(N);
}




void cse320_settimer(int newN) {
	N = newN;
	alarm(N);
}

int cse320_gettimer(){
	return N;

}


/*


   void printArray1(void* ptr){
   int j=0;
   for(j=0; j<25;j++){
   printf("At %d addr :%p, count: %d\n",j+1, (ptr+j)->addr, (ptr+j)->ref_count);
   }
   }



   void printArray2(void* ptr){
   int j=0;

   for(j=0; j<5;j++){
   printf("At %d filename :%s, count: %d, FP: %p ",j+1, (ptr+j)->filename, (ptr+j)->ref_count, (ptr+j)->fptr);

   if((ptr+j)->fptr!=NULL){
   printf("FD: %d\n",fileno((ptr+j)->fptr));

   }
   else {
   printf("FD:  \n"); 


   }
   }

   }
 */
