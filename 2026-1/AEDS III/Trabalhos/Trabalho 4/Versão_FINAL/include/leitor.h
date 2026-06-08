#ifndef LEITOR_H
#define LEITOR_H

// Estrutura para armazenar os dados brutos lidos do arquivo de instância
typedef struct {
    int n;                // Quantidade de elementos
    int *valores;         // Vetor dinâmico com os valores lidos
    long long soma_total; // Soma de todos os elementos (evita overflow)
} Instancia;

// Assinaturas das funções de manipulação de instância
Instancia* ler_instancia(const char *caminho_arquivo);
void liberar_instancia(Instancia *inst);

#endif // LEITOR_H