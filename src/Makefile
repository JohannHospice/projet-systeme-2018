CC=gcc
CFLAGS=-Wall -g -lpthread
EXEC=quicksort


all:$(EXEC)

sharing: lifo.o sharing.o quicksort.o
					 $(CC) -o sharing lifo.o sharing.o quicksort.o $(CFLAGS)

stealing: deque.o stealing.o quicksort.o
					 $(CC) -o stealing deque.o stealing.o quicksort.o $(CFLAGS)

quicksort.o: sched.h quicksort.c
						 $(CC) -o quicksort.o -c quicksort.c $(CFLAGS)

lifo.o: lifo.h lifo.c
				$(CC) -o lifo.o -c lifo.c	$(CFLAGS)

deque.o: deque.h deque.c
				$(CC) -o deque.o -c deque.c	$(CFLAGS)

sharing.o: sched.h sharing.c
					 $(CC) -o sharing.o -c sharing.c $(CFLAGS)

stealing.o: sched.h stealing.c
					 $(CC) -o stealing.o -c stealing.c $(CFLAGS)

clean:
	rm -rf *.o
mrproper: clean
	rm -rf  $(EXEC)
