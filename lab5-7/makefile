CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g

all: server client main

server:
	$(CC) $(CFLAGS) server.c -lzmq -o server.out

client:
	$(CC) $(CFLAGS) client.c -lzmq -o client.out

main: BalancedBinaryTree.o
	$(CC) $(CFLAGS) main.c -o main.out BalancedBinaryTree.o

BalancedBinaryTree:
	$(CC) $(CFLAGS) -c BalancedBinaryTree.c

clear:
	rm -f *.out *.so *.o