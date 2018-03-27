#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){

  char nom [40];

  int res = read(0, nom, 40);
  nom[res-1] = '\0';

  if(res < 0) {
    perror("Erreur read");
    exit(1);
  }

  FILE * f = fopen(nom,"r");

  if(f){
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    printf("Taille: %ld\n",size);
  }else{
    printf("Fichier introuvable\n");
  }
}
