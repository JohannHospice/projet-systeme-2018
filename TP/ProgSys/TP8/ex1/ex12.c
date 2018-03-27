#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char **argv){

  if(argc > 1){

    char *debut_mem = NULL;
    struct stat st;
    stat(argv[1], &st);
    int size = st.st_size;

    int paquet = 128*1024;

    int fd = open(argv[1],O_RDONLY);
    if(fd == -1){
      printf("Erreur open\n");
      return 0;
    }

    int cpt = 0;

    for(int i = 0;  i < (size/paquet)+1 ; i++ ){

      debut_mem = mmap(NULL,paquet,PROT_READ,MAP_PRIVATE,fd,0);

      if(debut_mem == MAP_FAILED){
        printf("Erreur de mmap\n");
        return 0;
      }
      // faire cas pour dernier paquet de taille inférieur à 128ko
      // mmap probléme si fichier < 128ko

      for(int j = 0 ; j < paquet ; j++ ){
        char c = debut_mem[j];
        if(c == '*'){
          cpt++;
        }
      }
      lseek(fd,paquet,SEEK_CUR);
      munmap(debut_mem,paquet);
    }

    printf("Nombre d'occurences de 42: %d\n",cpt);

  }
  else{
    printf("Pas de fichier renseigné\n");
  }
  return 0;


}
