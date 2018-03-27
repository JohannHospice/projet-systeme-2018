#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){

  int buf_size = 100;
  int ret = fork();

  if(ret == -1){
    printf("Error\n");
  }

  if(ret == 0){  // processus lit stdin et l'écrit dans le fichier

    int fd = open( "message" , O_CREAT | O_RDWR, 0666);
    char buffer[buf_size];
    int nb_lu = read(STDIN_FILENO, buffer, buf_size-1);
    buffer[buf_size-1] = '\0';
    write(fd,buffer,nb_lu);
    close(fd);
  }
  else {
    //processus pére lit le fichier et écrit sur stdout
    int pid;
    int g;

    while( (pid = wait(&g)) != -1 ){
      printf("Child process terminated\n");
    }

    int fd = open( "message" , O_RDWR , 0666);
    char buffer[buf_size];
    int nb_lu = read(fd,buffer,buf_size-1);
    write(STDOUT_FILENO,buffer,nb_lu);
    close(fd);
  }

}
