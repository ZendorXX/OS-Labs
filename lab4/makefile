CC = gcc
CFLAGS = -Wall -Wextra -pedantic

test_prog_1_1: realisation_1
	$(CC) $(CFLAGS) test_prog_1.c -o test_prog_1_1.out realisation_1.o

test_prog_1_2: realisation_2
	$(CC) $(CFLAGS) test_prog_1.c -o test_prog_1_2.out realisation_2.o

realisation_1: library1
	$(CC) $(CFLAGS) -shared -o libreal1.so realisation_1.o

realisation_2: library2
	$(CC) $(CFLAGS) -shared -o libreal2.so realisation_2.o

library1:
	$(CC) $(CFLAGS) -c realisation_1.c -fPIC

library2:
	$(CC) $(CFLAGS) -c realisation_2.c -fPIC

clear:
	rm -f *.out *.so *.o