#include "cse320_functions.h" 
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

struct addr_in_use addr_array[25];
struct files_in_use files_array[5];
int pidList[100];

// SAMPLE
int main(){

	cse320_set(addr_array, files_array);
	cse320_setPidList(pidList); // for reaping using thread

	/*DO NOT CHANGE ABOVE, YOUR TEST STARTS FROM HERE*/


	// Example
	cse320_free(cse320_malloc(30)); 

	cse320_fopen("A");
	cse320_fclose("A"); 

	// Default timer = 5
	cse320_fork();
	sleep(cse320_gettimer1());// Please include this line

	// Default timer = 5 
	cse320_fork_thread();
	sleep(cse320_gettimer2()+1); // Please include this line
	//Thread can take more time. If it's the case, change 1 to greater value.  


	/* DO NOT CHANGE BELOW, YOUR TEST FINISHES AFTER THIS*/
	cse320_clean();  


	return 1;
}




