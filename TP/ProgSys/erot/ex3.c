#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char **argv){

  char *debut_mem = NULL;

  if(argc > 1){
    int fd = open(argv[1],O_RDONLY);
    debut_mem = mmap(NULL,4096,PROT_WRITE,MAP_SHARED,fd,0);
  }
  else{
    printf("Pas de fichier renseigné\n");
  }
  return 0;

}
