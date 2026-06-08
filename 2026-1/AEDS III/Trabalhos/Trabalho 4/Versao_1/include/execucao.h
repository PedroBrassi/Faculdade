#ifndef EXECUCAO_H
#define EXECUCAO_H

#include <stdio.h>

typedef long long (*AlgorithmFunction)(int *, int, int *);

typedef struct {
    const char *name;
    AlgorithmFunction function;
} Algorithm;

void setResultsPath(const char *path);
void createResultsFolder();
FILE *openGeneralResults();
FILE *openAlgorithmFile(const char *algorithmName);
void runExecutions();

#endif