#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#define NUM_SIZES          7
#define INSTANCES_PER_SIZE 5
#define FIXED_SEED         42   /* seed fixo — instâncias sempre idênticas */

void createDirectory(const char *folderName) {
    struct stat st = {0};
    if (stat(folderName, &st) == -1)
        mkdir(folderName, 0700);
}

void generateInstance(const char *filename, int n, int minValue, int maxValue) {
    FILE *file = fopen(filename, "w");
    if (!file) { printf("Erro em criar: %s\n", filename); return; }
    fprintf(file, "%d\n", n);
    for (int i = 0; i < n; i++) {
        int value = rand() % (maxValue - minValue + 1) + minValue;
        fprintf(file, "%d\n", value);
    }
    fclose(file);
}

int main() {
    srand(FIXED_SEED);

    int sizes[NUM_SIZES] = {100, 200, 500, 1000, 2500, 5000, 10000};

    char folderName[100];
    char fileName[200];

    createDirectory("instances");

    for (int i = 0; i < NUM_SIZES; i++) {
        clock_t start = clock();

        sprintf(folderName, "instances/instancia_%d", sizes[i]);
        createDirectory(folderName);

        for (int j = 1; j <= INSTANCES_PER_SIZE; j++) {
            sprintf(fileName, "%s/instancia_%d_%d.txt", folderName, sizes[i], j);

            int minValue = 1;
            int maxValue = (sizes[i] >= 100000) ? 1000000 : 10000;

            generateInstance(fileName, sizes[i], minValue, maxValue);
        }

        double t = (double)(clock() - start) / CLOCKS_PER_SEC;
        printf("%d instancias de %d criadas em %.2fs\n", INSTANCES_PER_SIZE, sizes[i], t);
    }

    printf("\nTodas as instancias foram criadas! (seed=%d)\n", FIXED_SEED);
    return 0;
}
