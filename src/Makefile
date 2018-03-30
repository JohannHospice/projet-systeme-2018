CC=gcc
CFLAGS=-Wall -g
EXEC=quicksort


all:$(EXEC)

quicksort: lifo.o sharing.o quicksort.o
					 $(CC) -o quicksort lifo.o sharing.o quicksort.o $(CFLAGS)

quicksort.o: sched.h quicksort.c
						 $(CC) -o quicksort.o -c quicksort.c $(CFLAGS)

lifo.o: lifo.h lifo.c
				$(CC) -o lifo.o -c lifo.c	$(CFLAGS)

sharing.o: sched.h sharing.c
					 $(CC) -o sharing.o -c sharing.c $(CFLAGS)

clean:
	rm -rf *.o
mrproper: clean
	rm -rf  $(EXEC)
