#include "kernel/types.h"
#include "user/user.h"
 
int
main(int argc, char *argv[])
{
  printf("ticks desde o boot: %d\n", uptime());
  exit(0);
}
