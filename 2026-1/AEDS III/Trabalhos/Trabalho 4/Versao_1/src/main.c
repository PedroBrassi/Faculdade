#include <stdlib.h>
#include <time.h>

#include "execucao.h"

int main() {

    srand(time(NULL));

    runExecutions();

    int ret = system("python3 plot.py");

    if (ret != 0) {
        fprintf(stderr, "Falha ao executar plot.py\n");
    }

    return 0;
}