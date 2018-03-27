#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv){

  for(int i = 0 ; i < 13 ; i++){
    int ret = fork();
    if(ret == 0){
      printf("PID: %d\n",getpid());
      break;
    }
  }

}
