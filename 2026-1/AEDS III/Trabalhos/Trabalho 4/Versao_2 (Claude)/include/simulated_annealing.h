#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

void generateNeighbor(int current[], 
    int neighbor[], 
    int n);

long long simulatedAnnealing(int values[], 
    int n, 
    int solution[]);

#endif