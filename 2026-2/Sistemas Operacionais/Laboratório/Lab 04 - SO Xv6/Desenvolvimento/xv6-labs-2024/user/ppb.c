#include "kernel/types.h"
#include "user/user.h"
 
int
main(int argc, char *argv[])
{
  int pai_filho[2]; // pai escreve, filho le
  int filho_pai[2]; // filho escreve, pai le
  char byte = 'x';
 
  pipe(pai_filho);
  pipe(filho_pai);
 
  int pid = fork();
  if(pid == 0){
    // FILHO
    close(pai_filho[1]);
    close(filho_pai[0]);
    read(pai_filho[0], &byte, 1);
    printf("%d: received ping\n", getpid());
    // write(filho_pai[1], &byte, 1);
    close(pai_filho[0]);
    // close(filho_pai[1]);
    exit(0);
  } else {
    // PAI
    close(pai_filho[0]);
    // close(filho_pai[1]);
    write(pai_filho[1], &byte, 1);
    read(filho_pai[0], &byte, 1);
    printf("%d: received pong\n", getpid());
    close(pai_filho[1]);
    close(filho_pai[0]);
    wait(0);
    exit(0);
  }
}
