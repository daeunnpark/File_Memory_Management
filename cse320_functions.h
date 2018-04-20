#ifndef LIB   
#define LIB 

#include <stddef.h>
#include <stdio.h>

void *cse320_malloc(void* addr_array,size_t size);
void cse320_free(void* ptr);
FILE *cse320_fopen(char *filename);
void cse320_fclose(char *filename);
void cse320_clean();

void cse320_fork();
void cse320_reap(int signum);
void cse320_settimer(int newN);


//extern struct addr_in_use addr_array[25];
//extern struct files_in_use files_array[5];

/*
extern struct addr_in_use{  
void* addr; 
int ref_count;// = 0;
};

extern struct files_in_use{

char* filename; 
int ref_count;// = 0; 
FILE *fptr;
};
*/
#endif
