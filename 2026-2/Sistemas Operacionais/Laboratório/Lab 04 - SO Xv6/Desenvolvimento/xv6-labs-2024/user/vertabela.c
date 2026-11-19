#include "kernel/types.h"
#include "user/user.h"
 
#define PAG 4096
 
int
main(int argc, char *argv[])
{
  printf("== ANTES: memoria livre = %d paginas\n", freemem() / PAG);
  pgtprint();
 
  char *p = sbrk(3 * PAG); // pede 3 paginas novas
  p[0] = 'A'; // usa a memoria nova
  printf("\n== sbrk(3 paginas) devolveu o endereco %p\n", p);
 
  printf("== DEPOIS: memoria livre = %d paginas\n", freemem() / PAG);
  pgtprint();
  exit(0);
}
