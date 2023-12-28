CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g

all: server client main

server:
	$(CC) $(CFLAGS) server.c -lzmq -o server.out

client:
	$(CC) $(CFLAGS) client.c -lzmq -o client.out

main: calculationNode balancedBinaryTree.o
	$(CC) $(CFLAGS) main.c -o main.out balancedBinaryTree.o

calculationNode: timer.o
	$(CC) $(CFLAGS) calculationNode.c -o calculationNode.out timer.o

balancedBinaryTree:
	$(CC) $(CFLAGS) -c balancedBinaryTree.c

timer:
	$(CC) $(CFLAGS) -c timer.c

clear:
	rm -f *.out *.so *.o