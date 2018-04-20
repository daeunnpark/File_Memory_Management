# Homework #4

<!--Do not cheat and good luck!

It will be great (and useful for you) if you will document your work in this README and write a sort of documentation for your homework. We may give few extra credits for good documentation. -->

## Introduction



# Makefile
This is a sample Makefile.
```
all:
	gcc part1.c -o part1	
	gcc part23_test.c  -o part23 cse320_functions.o  -lpthread
	gcc A.c -o A
clean:
	rm -f part1 part23 A

```

\*Sample Makefile makes two executable files:
-  `"part1"` (from `"part1.c"`)
-  `"part23"` (from `"part23_test.c"`).

Sample file `"part23_test.c"` provides a guideline for part2 and part3.
`"part23_test.c"` include the library header file `"cse320_functions.h"`, which imports part2 and part3 functions. As you can see in `"part23_test.c"`, any test file should start by `cse320_set(struct addr_in_use* addr_array2, struct files_in_use* files_array2)` and finishes by `cse320_clean()`.

\* If you are creating your own Makefile, be sure to include library `"cse320_functions.o"` in your Makefile.


Read below description for each function in `"cse320_functions.h"` for part2 and part3.   


## Struct/Function Description

`void cse320_set(struct addr_in_use* addr_array2, struct files_in_use* files_array2)` <br /> 
Resets addr\_in\_use array and files\_in\_use array. Must be called before any function call. 

`void *cse320_malloc(size_t size)` <br />
Takes a size of type size\_t and allocate a block of memory that is size bytes large. If the requested memory can be allocated, a pointer of type void\* is returned to the beginning of the memory block.  
Besides, the number of memory block is limited to 25. If the user tries to allocate beyond the limit, the function prints the message "Not enough memory" and terminates with exit value -1. 

`void cse320_free(void* ptr)` <br />
Frees the memory space pointed to by ptr, which must have been returned by a previous call to `cse320_malloc()`.  
If there is no such address allocated, print the function prints the message ”Free: Illegal address” and terminates with exit value -1.  
If the address in the pointer is already freed, print the function prints the message ”Free: Double free attempt” and terminates with exit value -1.  

`FILE *cse320_fopen(char *filename)`<br />
Opens the file whose name is the string pointed to by filename and associates a stream with it, with default flag "w". Returns the file pointer associated.  
The same file can be opened multiples times, but have to be closed the same number of times. Otherwise, the file remains opened. The int variable ref\_count in files\_in\_use struct indicates the remaining times to close. If ref\_count is 0, the file is closed.   
Besides, the number of opened files is limited to 25. If the user tries to open beyond the limit. the function prints the message "Too many opened files" and terminates with exit value -1. The global int variable files\_count indicates the number of opened files.  

`void cse320_fclose(char *filename)`<br />
Closes the file whose name is the string pointed to by filename and associates a stream with it.  
For each file, the int variable ref\_count in files\_in\_use struct indicates the remaining times to close.  
If there is no such opened file, the function prints the message ”Close: Illegal filename” and terminates with exit value -1.   
If the file is already closed, the function prints the message ”Close: Ref count is zero” and terminates with exit value -1.   

`void cse320_clean()`<br />
Frees any allocated memory block and closes any opened file, before exit.  

`void cse320_fork()`<br />
Creates a new process(child) by duplicating the calling process(parent). When `cse320_fork()` is called for the first time, `cse320_fork()` sets an timer of N seconds (N = 5 by default, but modifiable by calling `cse320_settimer(int newN))`, which calls `cse320_reap()` every N seconds to reap child.

`void cse320_reap(int signum)`<br />
Waits all children and reap them, to avoid zoombie process. `cse320_reap(int signum)` is called every N seconds (initialized to 5 but modifiable with `cse320_settimer(int newN)`).

`void cse320_settimer(int newN)`<br />
Takes a parameter newN of type int, changes global int variable N to new N. N is the interval for the timer to call `cse320_reap(int signum)`.

`int cse320_gettimer()`<br />
Returns the current interval of the timer to call `cse320_reap(int signum)`.
<br /> 



@github/daeupark
