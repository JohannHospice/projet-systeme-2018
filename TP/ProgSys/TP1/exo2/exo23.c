#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

long taille_fichier(FILE * f){
  long taille = -1;
  if(f){
    fseek(f, 0, SEEK_END);
    taille = ftell(f);
  }
  return taille;
}

void lire_dossier(char * nom_dossier){

  DIR *d;
  struct dirent *dir;
  d = opendir(nom_dossier);

  if (d) {
    while ((dir = readdir(d)) != NULL) {
      char *chemin_absolu = malloc(strlen(nom_dossier) + strlen(dir->d_name) + 2);
      sprintf(chemin_absolu, "%s/%s", nom_dossier , dir->d_name);

      if(dir->d_type == DT_DIR && strcmp(dir->d_name,".") != 0 && strcmp(dir->d_name,"..")!=0 ){ // vérifie si c'est un dossier
        lire_dossier(chemin_absolu);
      }
      else if(dir->d_type !=DT_DIR){
      FILE * f = fopen(chemin_absolu,"r");
      if(f){
        long taille = taille_fichier(f);
        if(taille > 10000){
          printf("Fichier: %s  Taille: %ld\n",chemin_absolu,taille);
        }
        fclose(f);
      }else{
        printf("Erreur lors de la lecture du fichier %s." , dir->d_name);
      }
    }
    free(chemin_absolu);
  }
  closedir(d);
}

}

int main(int argc, char **argv ){
  char *dossier = NULL;
  if(argc > 1){
    dossier = argv[1];
  }
  lire_dossier(dossier);
}
