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

  int fd[2];
  pipe(fd);

  int pid = fork();

  if(pid == -1){
    printf("Error du fork\n");
    exit(0);
  }

  if(pid == 0){  //fils
    close(fd[0]);
    dup2( fd[1],STDOUT_FILENO);
    char * arg[] = {"w","-h",NULL};
    execv("/usr/bin/w",arg);
  }
  else { // pére
    close(fd[1]);
    dup2(fd[0],STDIN_FILENO);
    char * arg[] = {"wc","-l",NULL};
    execv("/usr/bin/wc",arg);
  }
}
