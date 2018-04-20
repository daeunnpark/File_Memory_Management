all:
	gcc part1.c -o part1	
	gcc test.c  -o part2 cse320_functions.o  -lpthread
	gcc A.c -o A
clean:
	rm -f part1 part2 A

