#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char **argv){
    fork();
    fork();
    fork();
    sleep(10);
    exit(0);

}
