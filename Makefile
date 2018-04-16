all:
	gcc main.c  -o part2 -lpthread
	gcc A.c -o A
clean:
	rm -f part2 A

