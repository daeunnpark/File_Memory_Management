#ifndef LIB   
#define LIB 

#include <stdio.h> 
void *cse320_malloc(size_t size);
int cse320_free(void* ptr);
FILE *cse320_fopen(char *filename);
void cse320_fclose(char *filename);
void cse320_clean();



#endif
