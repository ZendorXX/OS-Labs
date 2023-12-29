all: compilation

parser.o: parser.cpp
	g++ -c parser.cpp -o parser.o

main.o: main.cpp
	g++ -c main.cpp -o main.o

graph.o: graph.cpp
	g++ -c graph.cpp -o graph.o

compilation: parser.o graph.o main.o job.c
	gcc job.c -o job
	g++ main.o parser.o graph.o -o main

clean:
	rm *.o main job

