#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv){


int ret = fork();

if(ret == -1){
  printf("Erreur\n");
}

if(ret == 0){  //fils
  char * args[argc];

  for(int i = 1 ; i < argc ; i++){
    args[i-1] = argv[i] ;
  }

  args[argc-1] = NULL;
  execvp(args[0],args);
}

int g;
printf("PID:%d\n",ret);
wait(&g);

if(WIFEXITED(g) > 0){
  printf("Terminaison normale\n");
}
else if(WTERMSIG(g) > 0){
  printf("Tué par la signal %d\n",WSTOPSIG(g));
}
else {
  printf("Bizarre\n");
}

}
