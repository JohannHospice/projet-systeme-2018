#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){

  int ret = fork();

  if(ret == -1){
    printf("Erreur\n");
  }
  if(ret == 0){  //fils
    char * arg[] = {"ls","-l",NULL};
    execv("/bin/ls",arg);
  }
  wait(NULL);

}
