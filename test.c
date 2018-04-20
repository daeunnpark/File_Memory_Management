#include "cse320_functions.h" 
#include <stdio.h>
#include <unistd.h>

// test.c
//int addr_count=0;
//int files_count=0; // opened files
//int N=5;
//int reapflag=0;
//sem_t mutex;


struct addr_in_use addr_array[25];
struct files_in_use files_array[5];


void printArray1(struct addr_in_use* ptr){
	int j=0;
	for(j=0; j<25;j++){
		printf("At %d addr :%p, count: %d\n",j+1, (ptr+j)->addr, (ptr+j)->ref_count);
	}
}


void printArray2(struct files_in_use* ptr){
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






int main(){
	reset(addr_array, files_array);


	// PART2 AND PART3
	printf("TEST------\n");
	//printArray1(addr_array);

	cse320_fork();
	cse320_fork();

	cse320_settimer(1); 
	cse320_fork();
	cse320_settimer(3); 
	sleep(cse320_gettimer());// Default timer N = 5



	//	printArray1(addr_array);
	//	void *p = cse320_malloc(addr_array, 40);
	/*
	   for(int j=0; j<23; j++){
	   cse320_malloc(30); 
	   }
	   cse320_free(p);
	   cse320_malloc(30);
	   cse320_malloc(30);
	   cse320_malloc(30);
	 */
	//cse320_clean(addr_array, files_array);
	//	printArray1(addr_array); 
	//cse320_clean();

	return 1;
}




