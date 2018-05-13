CC=gcc
CFLAGS=-Wall -g -lpthread
EXEC=sharing stealing sharing_copymatrix stealing_copymatrix


all: $(EXEC)

sharing: lifo.o sharing.o quicksort.o
	$(CC) -o $@ $^ $(CFLAGS)

stealing: deque.o stealing.o quicksort.o
	$(CC) -o $@ $^ $(CFLAGS)

stealing_copymatrix: deque.o stealing.o matrix.o copymatrix.o
	$(CC) -o $@ $^ $(CFLAGS)

sharing_copymatrix: lifo.o sharing.o matrix.o copymatrix.o
	$(CC) -o $@ $^ $(CFLAGS)

lifo.o: lifo.c lifo.h
	$(CC) -o $@ -c $< $(CFLAGS)

deque.o: deque.c deque.h
	$(CC) -o $@ -c $< $(CFLAGS)

matrix.o: matrix.c matrix.h
	$(CC) -o $@ -c $< $(CFLAGS)

quicksort.o: quicksort.c sched.h
	$(CC) -o $@ -c $< $(CFLAGS)

copymatrix.o: copymatrix.c matrix.h sched.h
	$(CC) -o $@ -c $< $(CFLAGS)

sharing.o: sharing.c sched.h
	$(CC) -o $@ -c $< $(CFLAGS)

stealing.o: stealing.c sched.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o
mrproper: clean
	rm -rf $(EXEC)
