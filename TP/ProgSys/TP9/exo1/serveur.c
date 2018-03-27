#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>

// compiler : -lrt -lpthread

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

  ftruncate(fd,sizeof(struct rendezvous));

  rdv =  mmap(NULL,sizeof(struct rendezvous),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
  if(rdv == MAP_FAILED){
    printf("Erreur de mmap\n");
    return 0;
  }

  sem_init( &(rdv->vide),1,0);
  sem_init( &(rdv->plein),1,1);

  while(1){
    sem_wait( &(rdv->plein)); // decr
    printf("Value dans serveur: %d\n", rdv->value);
    sem_post( &(rdv->vide) ); // incr
  }
  
}
