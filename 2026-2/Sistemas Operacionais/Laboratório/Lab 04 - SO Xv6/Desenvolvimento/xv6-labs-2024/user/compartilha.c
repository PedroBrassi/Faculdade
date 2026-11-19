#include "kernel/types.h"
#include "user/user.h"
 
int
main(int argc, char *argv[])
{
  int pid = fork();
  if(pid == 0){
    printf("\n######## FILHO (pid %d) ########\n", getpid());
    pgtprint();
    exit(0);
  }
  wait(0); // o pai espera o filho imprimir primeiro
  printf("\n######## PAI (pid %d) ########\n", getpid());
  pgtprint();
  exit(0);
}
