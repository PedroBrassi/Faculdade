#include "kernel/types.h"
#include "user/user.h"
 
int
main(int argc, char *argv[])
{
  printf("Ola! Eu sou um processo do xv6. Meu PID e %d\n", getpid());
  exit(0);
}
