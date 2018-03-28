CC=gcc
CFLAGS=-Wall

default:

sharing: quicksort.o sharing.o
	$(CC) -o $@ $^

stealing: quicksort.o stealing.o
	$(CC) -o $@ $^

quicksort.o: quicksort.c
	$(CC) -o $@ -c $< $(CFLAGS)

sharing.o: sharing.c sched.h
	$(CC) -o $@ -c $< $(CFLAGS)

stealing.o: stealing.c sched.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o
mrproper: clean
	rm -rf  $(LIB)
