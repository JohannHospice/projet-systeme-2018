#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <string.h>

int main(){

  mkfifo("/tmp/logger", 0666);

  int fd;
  int buf_size = 100;
  char buf[buf_size];

  fd = open( "/tmp/logger", O_RDONLY);

  while(1){
    int ret = read( fd, buf, buf_size ) ;
    if(ret == 0 ){
      close(fd);
      fd = open( "/tmp/logger", O_RDONLY);
    }else if(ret == -1){
      printf("Erreur lors de la lecture de /tmp/logger\n");
      exit(0);
    }else{
      write(STDOUT_FILENO,buf,ret);
    }
  }

}
