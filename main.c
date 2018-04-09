#include "cse320_functions.h"
#include <unistd.h>
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
	FILE *fptr;
};


void printArray1(struct addr_in_use* ptr);
void printArray2(struct files_in_use* ptr);

struct addr_in_use addr_array[25];
struct files_in_use files_array[25];


int main(){

	//	printArray(addr_array);
	//	printArray2(files_array);
	//cse320_free(cse320_malloc(30)); 
	//printArray2(file_array);

	//	cse320_clean();
//	cse320_fopen("A");
//	cse320_fopen("A");
//cse320_fclose("A");
cse320_fclose("B");
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
	FILE *ptr = NULL;
	for(o=0; o<25;o++){
		if(files_array[o].filename!=NULL){
			if(strcmp(filename,files_array[o].filename) ==0 ){
				files_array[o].ref_count = files_array[o].ref_count+1; 
				//printf("F2: %d\n", fileno(files_array[o].fptr));	
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
		//printf("F1: %d\n", fileno(ptr)); 
		new_file->fptr = ptr;
		files_array[files_count]=*new_file;
		//printf("F01: %d\n", fileno(files_array[files_count].fptr)); 		
		files_count++;
	}
	return files_array[files_count-1].fptr;

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
				//	printf("SHOULD BE 0: %d\n",close(fileno(files_array[k].fptr)));					files_array[k].ref_count--;
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
	for(j=0; j<1;j++){



		// printf("F2: %d\n", fileno(files_array[j].fptr));
		//		printf("filename :%s, count: %d\n", (ptr+j)->filename, (ptr+j)->ref_count);

		printf("filename :%s, count: %d\n, F: %d\n", (ptr+j)->filename, (ptr+j)->ref_count, fileno((ptr+j)->fptr));
	}
}
