all: project3 test

project3: project3.c node0.c node1.c node2.c node3.c utilities.c
	gcc -Wall project3.c node0.c node1.c node2.c node3.c utilities.c -o project3

test: test.c
	gcc test.c -o test

clean:
	rm project3 test