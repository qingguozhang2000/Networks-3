project3: project3.c node0.c node1.c node2.c node3.c utilities.c
	gcc -g project3.c node0.c node1.c node2.c node3.c utilities.c -o project3

clean:
	rm project3
