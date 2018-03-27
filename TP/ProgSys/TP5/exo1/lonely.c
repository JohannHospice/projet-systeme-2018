#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <string.h>

// Q1

int main(){
  int max = 10;
  int cpt = 0;
  int fd =  open( "/tmp/lonely.lock" , O_CREAT | O_EXCL, 0666);
  if(fd == -1){
    return 127;
  }else{
    while(cpt < max){
      write(STDOUT_FILENO,"je suis seul",12);
      cpt++;
      sleep(2);
    }
  }
  remove("/tmp/lonely.lock");
}

// Q2 : le programme ne supprime pas le lockfile si on tue le programme
