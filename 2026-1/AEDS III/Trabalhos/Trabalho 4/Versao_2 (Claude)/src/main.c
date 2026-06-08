#include <stdlib.h>
#include <time.h>

#include "execucao.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));   /* aleatoriedade diferente a cada run — intencional */

    if (argc > 1)
        setResultsPath(argv[1]);

    runExecutions();
    return 0;
}
