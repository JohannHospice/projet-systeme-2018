#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>


int main(int argc, char **argv){

  for(int i = 0 ; i < 13 ; i++){
    int ret = fork();
    if(ret == -1){
      printf("Erreur\n");
    }
    if(ret == 0){
      printf("PID crée: %d\n",getpid());
      srand(time(NULL));
      int r = rand()%100;
      printf("Nombre au hasard : %d\n",r);
      break;
    }
  }

  int g;
  int pid;
  while( (pid = wait(&g)) != -1 ){
    printf("Retour: %d\n", WEXITSTATUS(g));
    printf("PID Zombie: %d\n",pid);
  }

}
