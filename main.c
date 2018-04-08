#include "cse320_functions.h"

#include "errno.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int addr_count=0;
int files_count;


struct addr_in_use{

	void* addr;
	int ref_count;// = 0;

};


struct files_in_use{

	char* filename;
	int ref_count;// = 0;

};


void printArray1(struct addr_in_use* ptr);
void printArray2(struct files_in_use* ptr);

struct addr_in_use addr_array[25];
struct files_in_use files_array[25];


int main(){

	//	printArray(addr_array);
	printArray2(files_array);
	cse320_free(cse320_malloc(30)); 
	//printArray2(file_array);

	cse320_clean();
	printArray2(files_array); 

}




void *cse320_malloc(size_t size){
	printf("malloc called\n");
	void *addr = malloc(size);
	struct addr_in_use *new_addr = malloc(sizeof(struct addr_in_use)); 

	new_addr->addr = addr;
	new_addr->ref_count++;
	addr_array[addr_count]= *new_addr;
	addr_count++;

	return addr;


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
				}
				else{// >0
					//printf("freed:!!\n");		
					free(ptr);					
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
	for(o=0; o<25;o++){
		if(files_array[o].filename!=NULL){
			if(strcmp(filename,files_array[o].filename) ==0 ){
				files_array[o].ref_count = files_array[o].ref_count+1; 
				return fopen(files_array[o].filename,"r" );
			}

		}

	}

	if(o==25){

		struct files_in_use* new_file = malloc(sizeof(struct files_in_use));

		strcpy(new_file->filename, filename);
		new_file->ref_count = new_file->ref_count+1;
		files_array[files_count]=*new_file;
		files_count++;
	}
	return fopen(files_array[o].filename, "r");

}







void cse320_fclose(char* filename){
	int k;
	//int found = 0;

	for(k=0; k<25; k++){
		if(files_array[k].filename!=NULL){
			if(strcmp(filename,files_array[k].filename) ==0){
				if(files_array[k].ref_count==0){
					printf("Close: Ref count is zero\n");
					errno=EINVAL;
					exit(-1);
				}
				else{// >0
					//printf("Closed:!!\n");		
					//free(ptr);					
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



void printArray1(struct addr_in_use* ptr){
	int j=0;
	for(j=0; j<25;j++){
		printf("addr :%p, count: %d\n", (ptr+j)->addr, (ptr+j)->ref_count);
	}
}



void printArray2(struct files_in_use* ptr){
	int j=0;
	for(j=0; j<25;j++){
		printf("addr :%s, count: %d\n", (ptr+j)->filename, (ptr+j)->ref_count);
	}
}
