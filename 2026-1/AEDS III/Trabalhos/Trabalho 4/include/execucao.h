#ifndef EXECUCAO_H
#define EXECUCAO_H

#include <stdio.h>

typedef long long (*AlgorithmFunction)(int values[],
    int n,
    int solution[]
);

typedef struct {

    char name[100];
    AlgorithmFunction function;

} Algorithm;

void createResultsFolder();

FILE *openGeneralResults();

FILE *openAlgorithmFile(
    const char *algorithmName
);

void runExecutions();

#endif