all:
	gcc part1.c -o part1	
	gcc part23_test.c  -o part23 cse320_functions.o  -lpthread
	gcc A.c -o A
clean:
	rm -f part1 part23 A

