#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char **argv){


  int fd = open("/tmp/logger", O_WRONLY);
  for(int i = 0 ; i < argc ; i++){
    write(fd,argv[i],strlen(argv[i]));
    write(fd,"\n",1);
  }


}
