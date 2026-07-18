#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

// Macro para tratar a criação de pastas no Windows ou Linux/Mac
#ifdef _WIN32
    #include <direct.h>
    #define CRIAR_PASTA(pasta) _mkdir(pasta)
#else
    #define CRIAR_PASTA(pasta) mkdir(pasta, 0777)
#endif

int main() {
    // Define o seed fixo para garantir a reprodutibilidade dos resultados
    srand(42);

    // Cria a pasta "instancias" um nível acima da pasta "tools"
    CRIAR_PASTA("../instancias");

    // Vetor com os tamanhos solicitados
    int tamanhos[] = {100, 200, 500, 1000, 2500, 5000, 10000};
    int qte_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);

    // 5 limites de valores diferentes para dar características distintas a cada arquivo
    int max_valores_por_tipo[] = {100, 1000, 5000, 25000, 100000};

    printf("==================================================\n");
    printf("  Gerador Automatico de Instancias \n");
    printf("==================================================\n");
    printf("Criando arquivos em '../instancias/'...\n\n");

    // Loop pelos tamanhos de n
    for (int i = 0; i < qte_tamanhos; i++) {
        int n = tamanhos[i];

        // Loop para criar as 5 instâncias de cada tamanho
        for (int k = 0; k < 5; k++) {
            int max_val = max_valores_por_tipo[k];
            char nome_arquivo[150];

            // instancia_tamanho_numero.txt
            sprintf(nome_arquivo, "../instancias/instancia_%d_%d.txt", n, k + 1);

            FILE *arquivo = fopen(nome_arquivo, "w");
            if (arquivo == NULL) {
                printf("Erro crítico ao criar o arquivo: %s\n", nome_arquivo);
                return 1;
            }

            // Grava a quantidade de elementos 'n' na primeira linha
            fprintf(arquivo, "%d\n", n);

            // Grava os 'n' elementos
            for (int j = 0; j < n; j++) {
                int valor = (rand() % max_val) + 1;
                fprintf(arquivo, "%d\n", valor);
            }

            fclose(arquivo);
            printf(" -> Arquivo '%s' gerado com sucesso.\n", nome_arquivo);
        }
    }

    printf("\n==================================================\n");
    printf("    Todas as instancias foram criadas!.\n");
    printf("==================================================\n");

    return 0;
}