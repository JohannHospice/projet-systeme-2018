#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

long taille_fichier(FILE * f){
  long taille = -1;
  if(f){
    fseek(f, 0, SEEK_END);
    taille = ftell(f);
  }
  return taille;
}

int main(int argc, char **argv ){
  char *dossier = NULL;
  if(argc > 1){
    dossier = argv[1];
  }

  DIR *d;
  struct dirent *dir;
  d = opendir(dossier);
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      if(dir->d_type == DT_REG ){ // vérifie si c'est un fichier
        FILE * f = fopen(dir->d_name,"r");
        if(f){
          long taille = taille_fichier(f);
          if(taille > 10000){
            printf("Fichier: %s  Taille: %ld\n",dir->d_name,taille);
          }
        }else{
            printf("Erreur lors de la lecture du fichier %s." , dir->d_name);
        }
      }
    }
    closedir(d);
  }
}
