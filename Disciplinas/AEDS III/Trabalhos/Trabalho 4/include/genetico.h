#ifndef GENETICO_H
#define GENETICO_H

#include "leitor.h"
#include "partition.h"

// Executa o Algoritmo Genético para uma instância e retorna a melhor solução encontrada
Solucao executar_algoritmo_genetico(const Instancia *inst);

#endif // GENETICO_H