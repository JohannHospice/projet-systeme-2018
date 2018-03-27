#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(){


  int buf_size = 80;
  char buffer[buf_size];
  int end;

  while(1){

    int res = read(0, buffer, buf_size-1);
    buffer[79] = '\n';
    if(res < 0) {
      perror("Erreur read");
      exit(1);
    }
    end = res;


    res = 0;
    while(res != end){
        res = res + write(1,buffer+res,end-res);
        if(res < 0) {
          perror("Erreur write");
          exit(1);
        }
    }
    end = 0;
  }
}
