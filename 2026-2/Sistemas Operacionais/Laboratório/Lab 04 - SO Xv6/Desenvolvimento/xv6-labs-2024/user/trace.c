#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc < 3){
    fprintf(2, "Uso: trace mascara comando [argumentos...]\n");
    exit(1);
  }

  if(trace(atoi(argv[1])) < 0){
    fprintf(2, "trace: erro ao ativar rastreamento\n");
    exit(1);
  }

  exec(argv[2], &argv[2]);

  fprintf(2, "trace: falha ao executar %s\n", argv[2]);
  exit(1);
}
