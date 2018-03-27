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

  char *regex = argv[1];

  int fd[2];
  int total = 0;
  pipe(fd);

  for(int i = 2 ; i < argc  ; i++ ){

    int pid = fork();

    if(pid == -1){
      printf("Error du fork\n");
      exit(0);
    }

    if(pid == 0){  //fils

      close(fd[0]);
      dup2( fd[1] , STDOUT_FILENO);
      char * arg[] = {"grep","-c",regex,argv[i],NULL};
      execv("/bin/grep",arg);

    }
    else { // pére
      close(fd[1]);
      int pid;
      int g;
      int buf_size = 20;
      char buffer[buf_size];

      while( (pid = wait(&g)) != -1 ){
        read(fd[0],buffer,buf_size-1);
        printf("PERE: %d",atoi(buffer));
      }
      printf("Total d'occurences: %d\n",total);
    }

  }


}
