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

  int fd = open( "compteur" , O_CREAT | O_RDWR, 0666);
  write(fd,"0",1);

  for(int i = 0 ; i < 30 ; i++){
    lseek(fd,SEEK_SET,0); // retourne au début

    char buffer[12];
    int nb_lu = read(fd,buffer,11);
    buffer[nb_lu] = '\0';

    int lu = atoi(buffer);
    printf("LU1: %d\n",lu);
    lu +=1;
    sprintf(buffer,"%d",lu); // écrit l'entier lu dans le buffer avec \0 à la fin

    lseek(fd,SEEK_SET,0);
    write(fd,buffer,strlen(buffer));
  }

}
