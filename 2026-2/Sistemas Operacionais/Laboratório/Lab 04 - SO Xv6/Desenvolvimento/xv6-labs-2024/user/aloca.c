#include "kernel/types.h"
#include "user/user.h"
 
#define PAG 4096
 
int
main(int argc, char *argv[])
{
  if(argc < 2){
    printf("uso: aloca <numero de paginas>\n");
    exit(1);
  }
  int n = atoi(argv[1]);
  int antes = freemem() / PAG;
  char *p = sbrk(n * PAG);
  int depois = freemem() / PAG;
  printf("pedi %d paginas a partir de %p\n", n, p);
  printf("livres: %d -> %d (sairam %d)\n", antes, depois, antes-depois);
  exit(0);
}
