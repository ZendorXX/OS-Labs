CC = g++
FLAGS = -Wall -Wextra -ggdb
ZMQ = `pkg-config --libs libzmq`


main: main.cpp btree.hpp child
	$(CC) $(FLAGS) -o main.out main.cpp btree.hpp $(ZMQ)

child: child.cpp timer.o
	$(CC) $(FLAGS) -o child.out child.cpp timer.o $(ZMQ)

timer:
	$(CC) $(FLAGS) -c timer.cpp
	
clean:
	rm -rf *.out