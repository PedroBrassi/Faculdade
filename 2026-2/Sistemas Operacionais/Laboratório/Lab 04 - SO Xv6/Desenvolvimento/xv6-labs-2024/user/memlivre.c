#include "kernel/types.h"
#include "user/user.h"
 
#define PAG 4096
 
int
main(int argc, char *argv[])
{
  int m0 = freemem();
  printf("[1] inicio: %d pags livres\n", m0/PAG);
 
  sbrk(10 * PAG); // pede 10 paginas a mais
  int m1 = freemem();
  printf("[2] sbrk: %d livres (-%d)\n", m1/PAG, (m0-m1)/PAG);
 
  int pid = fork();
  if(pid == 0){
    int m2 = freemem();
    printf("[3] filho: %d livres (-%d)\n", m2/PAG, (m1-m2)/PAG);
    exit(0);
  }
  wait(0);
  int m3 = freemem();
  printf("[4] fim: %d livres (-%d desde [2])\n", m3/PAG, (m1-m3)/PAG);
  exit(0);
}
