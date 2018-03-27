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

  int max = 10;
  int cpt = 0;
  const char* lockfile = "/tmp/lonely.lock";
  int fd = open(lockfile,O_CREAT,0666);

  flock(fd,LOCK_EX);

  struct stat * buf_file = malloc(sizeof(stat));
  struct stat * buf_fd = malloc(sizeof(stat));

  stat(lockfile,buf_file);
  fstat(fd,buf_fd);

  while(buf_file->st_ino != buf_fd->st_ino){
    fd = open(lockfile,O_CREAT,0666);
    stat(lockfile,buf_file);
    fstat(fd,buf_fd);
    flock(fd,LOCK_EX);
  }

  while(cpt < max){
    write(STDOUT_FILENO,"je suis seul",12);
    cpt++;
    sleep(2);
  }

  unlink(lockfile);
  flock(fd,LOCK_UN);

}
