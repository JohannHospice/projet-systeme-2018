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

  int fd1[2];
  int fd2[2];

  int ret = pipe(fd1);
  if(ret == -1 ){
    printf("Erreur lors de la création du pipe du pére");
    exit(0);
  }

  int ret2 = pipe(fd2);
  if(ret2 == -1 ){
    printf("Erreur lors de la création du pipe du fils");
    exit(0);
  }

  int pid = fork();
  if(pid == -1){
    printf("Error du fork\n");
    exit(0);
  }

  if(pid == 0){  //fils

    int buf_size = 100;
    char buffer[buf_size];
    int nb_lu = read(fd1[0], buffer, buf_size-1);
    write(fd2[1],"Salut papa",10);

  }
  else { // pére

    write(fd1[1],"Salut fiston",12);
    int buf_size = 100;
    char buffer[buf_size];
    int nb_lu = read(fd2[0], buffer, buf_size-1);
  }
}
