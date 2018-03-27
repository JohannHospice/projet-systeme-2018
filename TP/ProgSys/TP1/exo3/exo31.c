#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char **argv){

  int pid = fork();
  if(pid == -1){
    printf("Erreur\n");
    exit(0);
  }
  else if(pid == 0){ // fils
    char * chaine = "Je suis le fils\n";
    for(int i = 0 ; i < 10000 ; i++){
      write(1,chaine,strlen(chaine));
    }
  }
  else{  // pére
    char * chaine = "Je suis le pére\n";
    for(int i = 0 ; i < 10000 ; i++){
      write(1,chaine,strlen(chaine));
    }
  }
  int pid_zombie =  wait(NULL);
  printf("PID Zombie: %d\n",pid_zombie);

}
