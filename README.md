# Homework #4

<!--Do not cheat and good luck!

It will be great (and useful for you) if you will document your work in this README and write a sort of documentation for your homework. We may give few extra credits for good documentation. -->

## Introduction
This file contains guideline and explanations for part2 (including EC2.1 and 2.2) and part3 (including EC4) of HW#4. For Function description, following the sample file `"part23_test.c"` can be helpful.

# Makefile
This is a \*sample Makefile.
```
all:
	gcc part1.c -o part1	
	gcc part23_test.c  -o part23 cse320_functions.o  -lpthread
	gcc A.c -o A
clean:
	rm -f part1 part23 A

```

Two executable files are created:
-  `"part1"` (from `"part1.c"`)
-  `"part23"` (from `"part23_test.c"`).

Sample file `"part23_test.c"` provides a guideline for part2 and part3.
`"part23_test.c"` include the library header file `"cse320_functions.h"`, which imports part2 and part3 functions. As you can see in `"part23_test.c"`, any test file should start by `cse320_set(struct addr_in_use* addr_array2, struct files_in_use* files_array2)` and finishes by `cse320_clean()`.

\* If you are creating your own Makefile, be sure to include library `"cse320_functions.o"` in your Makefile.


Read below description for each function in `"cse320_functions.h"` for part2 and part3.   


## Function Description

`void cse320_set(struct addr_in_use* addr_array2, struct files_in_use* files_array2)` <br /> 
Resets `addr_in_use array` and `files_in_use array`. Must be called before any function call. 

`void *cse320_malloc(size_t size)` <br />
Takes a size of type `size_t` and allocates a block of memory that is size bytes large. If the requested memory can be allocated, a pointer of type `void*` is returned to the beginning of the memory block.  
Besides, the number of memory block is limited to 25. If the user tries to allocate beyond the limit, the function prints the message "Not enough memory" and terminates with exit value -1. 

`void cse320_free(void* ptr)` <br />
Frees the memory space pointed to by `ptr`, which must have been returned by a previous call to `cse320_malloc()`.  
If there is no such address allocated, the function prints the message ”Free: Illegal address” and terminates with exit value -1.  
If the address in the pointer is already freed, print the function prints the message ”Free: Double free attempt” and terminates with exit value -1.  

`FILE *cse320_fopen(char *filename)`<br />
Opens the file whose name is the string pointed to by filename and associates a stream with it, with default flag "w". Returns the file pointer associated.  
The same file can be opened multiples times, but have to be closed the same number of times. Otherwise, the file remains opened. The variable `int ref_count` in `struct files_in_use` indicates the remaining times to close. If `int ref_count` is 0, the file is closed.   
Besides, the number of opened files is limited to 25. If the user tries to open beyond the limit. The function prints message "Too many opened files" and terminates with exit value -1. The global variable `int files_count` indicates the number of opened files.  

`void cse320_fclose(char *filename)`<br />
Closes the file whose name is the string pointed to by filename and associates a stream with it.  
For each file, the variable `int ref_count` in `struct files_in_use` indicates the remaining times to close.  
If there is no such opened file, the function prints the message ”Close: Illegal filename” and terminates with exit value -1.   
If the file is already closed, the function prints the message ”Close: Ref count is zero” and terminates with exit value -1.   

`void cse320_clean()`<br />
Frees any allocated memory block and closes any opened file, before exit.  

`void cse320_fork()`<br />
Creates a new process(child) by duplicating the calling process(parent). When `cse320_fork()` is called for the first time, `cse320_fork()` sets a timer of N1 seconds (N1 = 5 by default, but modifiable by calling `cse320_settimer1(int newN)`), which calls `cse320_reap()` every N1 seconds to reap children using Signal until program finishes.  
This function should be followed by `sleep(int n)`.  

`void cse320_reap(int signum)`<br />
Waits all children and reap them, to avoid zoombie process. `cse320_reap(int signum)` is executed every N1 seconds (initialized to 5 but modifiable with `cse320_settimer1(int newN)`), until program finishes.  
 
`void cse320_settimer1(int newN)`<br />
Takes a parameter newN and changes the global variable `int N1` to newN. N1 is the interval for the timer to call `cse320_reap(int signum)`.

`int cse320_gettimer1()`<br />
Returns the current interval of the timer N1 to call `cse320_reap(int signum)`.

`void *cse320_setPidList(int* pidList)`<br />
Resets the address of the data structure `int pidList[]`. This data structure contains pid of children. For the sake of the homework, its capacity is limited to 10 pids, therefore 10 children. List of pids pidList is used to reap children in `void cse320_reap_thread()`. 

`void cse320_fork_thread()`<br />
Creates a new process(child) by duplicating the calling process(parent). When `cse320_fork_thread()` is called for the first time, `cse320_fork()` sets a timer of N2 seconds (N2 = 5 by default, but modifiable by calling `cse320_settimer2(int newN)`), which creates a thread calling by `cse320_reap_thread()`, reaping children  every N2 seconds until program finishes.  
When a child process is created, its pid is added to the `pidList[]`.  
This function should be followed by `sleep(int n)`.  

`void cse320_reap_thread()`<br />
Waits all children and reap them, to avoid zoombie process. `cse320_reap_thread()` is executed every N2 seconds (initialized to 5 but modifiable with `cse320_settimer2(int newN)`), until program finishes.  
When the a child process is reaped, its pid is removed from pidList.

`void cse320_settimer2(int newN)`<br />
Takes a parameter newN of and changes global variable `int N2` to newN. N2 is the interval for the timer to call `cse320_reap_thread()`.

`int cse320_gettimer2()`<br />
Returns the current interval of the timer N2 to call `cse320_reap_thread()`.

<br /> 


@github/daeupark
