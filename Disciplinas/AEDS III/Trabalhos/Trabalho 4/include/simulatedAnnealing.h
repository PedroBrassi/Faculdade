#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

#include "leitor.h"
#include "partition.h"

// Executa o algoritmo Simulated Annealing para uma instância e retorna a melhor solução encontrada
Solucao executar_simulated_annealing(const Instancia *inst);

#endif // SIMULATED_ANNEALING_H