# Homework #4
Do not cheat and good luck!

It will be great (and useful for you) if you will document your work in this README and write a sort of documentation for your homework. We may give few extra credits for good documentation.


main.c
void *cse320_malloc(size_t size);
Takes a size of type size_t and allocate a block of memory that is size bytes large. If the requested memory can be allocated, a pointer of type void* is returned to the beginning of the memory block.

Besides, the number of memory block is limited to 25. If the user tries to allocate beyond the limit, the function prints the message ""Not enough memory" and terminates with exit value -1.









void cse320_fork();
cse320_fork() creates a new process(child) by duplicating the calling process(parent).  When cse320_fork() is called for the first time, cse320_fork() sets an timer of N seconds, which alarm cse320_reap() every N secondsto reap child.


void cse320_reap(int signum);
cse320_reap(int signum) waits all children and reap them, to avoid zoombie process. cse320_reap(int signum) is called every N seconds, initialized to 10 but modifiable with cse320_settimer(int newN).

void cse320_settimer(int newN);
Takes a parameter new N of type int, change global int variable N to new N. N is the interval for the timer to call cse320_reap(int signum).



