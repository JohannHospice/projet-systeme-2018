CC=gcc
CFLAGS=-Wall -g -lpthread
EXEC=sharing stealing


all: $(EXEC)

sharing: lifo.o sharing.o quicksort.o
	$(CC) -o $@ $^ $(CFLAGS)

stealing: deque.o stealing.o quicksort.o
	$(CC) -o $@ $^ $(CFLAGS)

stealing_copymatrix: deque.o stealing.o copymatrix.o
	$(CC) -o $@ $^ $(CFLAGS)

sharing_copymatrix: lifo.o sharing.o copymatrix.o
	$(CC) -o $@ $^ $(CFLAGS)

lifo.o: lifo.c lifo.h
	$(CC) -o $@ -c $< $(CFLAGS)

deque.o: deque.c deque.h
	$(CC) -o $@ -c $< $(CFLAGS)

quicksort.o: quicksort.c sched.h
	$(CC) -o $@ -c $< $(CFLAGS)

copymatrix.o: copymatrix.c sched.h
	$(CC) -o $@ -c $< $(CFLAGS)


sharing.o: sharing.c sched.h
	$(CC) -o $@ -c $< $(CFLAGS)

stealing.o: stealing.c sched.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o
mrproper: clean
	rm -rf $(EXEC)
