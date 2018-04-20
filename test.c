#include "cse320_functions.h" 
#include <stdio.h>
#include <unistd.h>


int addr_count=0;
int files_count=0; // opened files
int N=5;
int reapflag=0;
//sem_t mutex;
char* command;
char* X;

struct addr_in_use{

	void* addr;
	int ref_count;// = 0;

};

struct files_in_use{

	char* filename;
	int ref_count;// = 0;
	FILE *fptr;
};


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
	// PART2 AND PART3
	printf("TEST------\n");


	//cse320_malloc(10);
	/*cse320_fork();
	  cse320_fork();
	  cse320_fork();
	  sleep(15);
	 */


	printArray1(addr_array);
	void *p = cse320_malloc(addr_array, 40);
	for(int j=0; j<23; j++){
	//	cse320_malloc(30); 
	}
	printArray1(addr_array); 
	printf("aaaaa1\n");
	return 1;
}




