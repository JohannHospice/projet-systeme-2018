#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>

struct rendezvous{
  int value;
  sem_t  vide;
  sem_t  plein;
};

int main(int argc, char **argv){

struct rendezvous * rdv;

int fd = shm_open("/mem", O_RDWR|O_CREAT , S_IRUSR | S_IWUSR );

if(fd == -1 ){
  printf("Erreur du shm_open\n");
  return 0;
}

rdv =  mmap(NULL,sizeof(struct rendezvous),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

if(rdv == MAP_FAILED){
  printf("Erreur de mmap\n");
  return 0;
}

sem_wait( &(rdv->vide)); //decr
rdv->value = 9;
sem_post( &(rdv->plein)); // incr

}
