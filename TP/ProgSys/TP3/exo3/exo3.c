#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){

  // à revoir

  int ret = fork();

  if(ret == -1){
    printf("Erreur\n");
  }

  if(ret == 0){  //fils écrit b

    int fd = open( "foo.txt" , O_CREAT | O_APPEND | O_RDWR, S_IRUSR | S_IRGRP | S_IROTH);
    for(int i = 0 ; i < 10000 ; i++) {
      //lseek(fd, 0,SEEK_END);
      write(fd,"b",1);
    }
    close(fd);
  }


  int fd = open( "foo.txt" , O_CREAT | O_APPEND | O_RDWR, S_IRUSR | S_IRGRP | S_IROTH);
  for(int i = 0 ; i < 10000 ; i++) {
    //lseek(fd, 0,SEEK_END);
    write(fd,"a",1);
  }
  close(fd);
}
