#ifndef	DEQUE_H
#define DEQUE_H
#endif


typedef struct Element Element;
struct Element{
  void* f;
  void* arg;
  Element *next;
  Element *prev;
};

typedef struct Deque Deque;
struct Deque{
  Element *head;
  Element *tail;
  size_t size;
};

int init(Deque *d);
int append(Deque *d,void *f,void* arg);
int prepend(Deque *d,void *f,void* arg);
int insert(Deque *d,void *f,void* arg,int index);
int display(Deque *d);
Element *remove_index(Deque *d, int index);
int free_deque(Deque *d);