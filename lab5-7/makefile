CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g

all: server client main

server:
	$(CC) $(CFLAGS) server.c -lzmq -o server.out

client:
	$(CC) $(CFLAGS) client.c -lzmq -o client.out

main: BalancedBinaryTree.o timer.o
	$(CC) $(CFLAGS) main.c -o main.out BalancedBinaryTree.o timer.o

BalancedBinaryTree:
	$(CC) $(CFLAGS) -c BalancedBinaryTree.c

timer:
	$(CC) $(CFLAGS) -c timer.c

clear:
	rm -f *.out *.so *.o