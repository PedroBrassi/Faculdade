#include "kernel/types.h"
#include "user/user.h"
 
int
main(int argc, char *argv[])
{
  if(argc < 2){
    printf("uso: falta 1 | 2 | 3 | 4 <endereco em decimal>\n");
    exit(1);
  }
  int op = atoi(argv[1]);
 
  if(op == 1){ // 1) ler um endereco nao mapeado
    int *longe = (int *) 0x40000000; // 1 GB: alem do processo
    printf("lendo 0x40000000...\n");
    printf("valor = %d\n", *longe);
  } else if(op == 2){ // 2) escrever no proprio codigo
    char *codigo = (char *) main; // endereco da funcao main
    printf("escrevendo em main (%p)...\n", codigo);
    codigo[0] = 0;
  } else if(op == 3){ // 3) ler e escrever no heap: ok
    char *p = sbrk(4096);
    p[0] = 'x';
    printf("heap ok: escrevi e li o heap sem problemas\n");
  } else if(op == 4 && argc == 3){ // 4) ler endereco dado
    int *e = (int *) (uint64) atoi(argv[2]);
    printf("lendo %p...\n", e);
    printf("valor = %d\n", *e);
  }
  printf("fim normal do programa\n");
  exit(0);
}
