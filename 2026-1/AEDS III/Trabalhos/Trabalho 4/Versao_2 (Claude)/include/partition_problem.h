#ifndef PARTITION_PROBLEM_H
#define PARTITION_PROBLEM_H

void      generateRandomSolution(int solution[], int n);
void      greedySolution(int values[], int solution[], int n);
long long evaluateFitness(int values[], int solution[], int n);
long long randomPartition(int values[], int n, int solution[]);
long long greedyPartition(int values[], int n, int solution[]);
void      printPartition(int values[], int solution[], int n);

#endif
