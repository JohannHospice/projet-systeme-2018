#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>

int main(){
  int max = 10;
  int cpt = 0;

  int fd =  open( "/tmp/lonely.lock" , O_CREAT , 0666);
  if(fd != -1){
    int r = flock(fd, LOCK_EX|LOCK_NB);
    if(r == 0){
      while(cpt < max){
        write(STDOUT_FILENO,"je suis seul",12);
        cpt++;
        sleep(2);
      }
      close(fd);
      flock(fd,LOCK_UN);
    }
    else{
      close(fd);
      return 127;
    }
  }
}
