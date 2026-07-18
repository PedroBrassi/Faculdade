/*
============================================================
  UNIVERSIDADE FEDERAL DE ALFENAS – UNIFAL-MG
  Bacharelado em Ciência da Computação
  Disciplina: Algoritmos e Estruturas de Dados II
  Professor: Iago Augusto de Carvalho
------------------------------------------------------------
  Trabalho Prático 2 – Ordenação em Memória Secundária
------------------------------------------------------------
  Integrantes do grupo:
  • Gustavo Silva Orlando – Matrícula: 2024.2.08.009
  • Pedro Brassi Luccas – Matrícula: 2022.2.08.009
------------------------------------------------------------
  Descrição:
  Implementação de um algoritmo de ordenação externa em C,
  que gera um arquivo binário de tamanho variável (em GB),
  divide-o em blocos de até 1 GB, ordena-os em memória
  principal utilizando Merge Sort e realiza a fusão externa
  dos blocos para produzir arquivos finais ordenados.

  O programa também gera um relatório de execução com:
  - Tamanho total de dados processados
  - Quantidade de blocos criados
  - Tempo de geração do arquivo
  - Tempo de ordenação e tempo total

  Saídas finais:
  • resultado_final.bin – arquivo binário ordenado
  • resultado_final.txt – arquivo texto ordenado
  • relatorio_execucao.txt – relatório textual
============================================================
*/

#include "ordenacaoExterna.h"

// ==================
// Programa principal
// ==================
int main() {
    char nomeArquivo[100] = "dados.bin";
    double tamanhoGB;
    long long totalBytes, totalInteiros;
    clock_t inicioTotal, fimTotal, inicioGeracao, fimGeracao, inicioOrdenacao, fimOrdenacao;

    printf("=== TRABALHO AEDS II ===\n");
    printf("Informe o tamanho do arquivo a ser criado (em GB): ");
    if (scanf("%lf", &tamanhoGB) != 1) {
        printf("Erro na leitura do valor!\n");
        return 1;
    }

    inicioTotal = clock();

    totalBytes = (long long)(tamanhoGB * BYTES_POR_GB);
    totalInteiros = totalBytes / sizeof(int);

    // ============ Etapa 1: Gerar arquivo principal ============
    FILE *arquivo = fopen(nomeArquivo, "wb");
    if (!arquivo) {
        printf("Erro ao criar arquivo!\n");
        return 1;
    }

    srand(time(NULL));

    const long long bloco = 1000000;
    int *buffer = (int *) malloc(bloco * sizeof(int));
    if (!buffer) {
        printf("Erro de memória!\n");
        fclose(arquivo);
        return 1;
    }

    printf("\nGerando arquivo de %.2f GB (%lld inteiros)...\n", tamanhoGB, totalInteiros);

    inicioGeracao = clock();
    for (long long i = 0; i < totalInteiros; i += bloco) {
        long long qtd = (i + bloco <= totalInteiros) ? bloco : (totalInteiros - i);
        for (long long j = 0; j < qtd; j++)
            buffer[j] = rand();
        fwrite(buffer, sizeof(int), qtd, arquivo);
        if (i % (bloco * 100) == 0)
            printf("%.2f%% concluído...\n", (100.0 * i) / totalInteiros);
    }
    fimGeracao = clock();

    free(buffer);
    fclose(arquivo);
    printf("Arquivo '%s' gerado com sucesso!\n\n", nomeArquivo);

    // ============ Etapa 2: Dividir e ordenar blocos de até 1 GB ============
    inicioOrdenacao = clock();

    long long tamanhoBlocoBytes = BYTES_POR_GB;
    long long inteirosPorBloco = tamanhoBlocoBytes / sizeof(int);

    arquivo = fopen(nomeArquivo, "rb");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }

    int *dados = (int *) malloc(inteirosPorBloco * sizeof(int));
    if (!dados) {
        printf("Erro ao alocar memória!\n");
        fclose(arquivo);
        return 1;
    }

    int contadorBlocos = 0;
    size_t lidos;

    printf("Dividindo e ordenando blocos...\n");

    while ((lidos = fread(dados, sizeof(int), inteirosPorBloco, arquivo)) > 0) {
        contadorBlocos++;

        printf("Ordenando bloco %d (%zu inteiros)...\n", contadorBlocos, lidos);
        mergeSort(dados, 0, lidos - 1);

        // Cria o bloco binário
        char nomeBlocoBin[50], nomeBlocoTxt[50];
        sprintf(nomeBlocoBin, "bloco_ordenado_%d.bin", contadorBlocos);
        sprintf(nomeBlocoTxt, "bloco_ordenado_%d.txt", contadorBlocos);

        FILE *blocoBin = fopen(nomeBlocoBin, "wb");
        FILE *blocoTxt = fopen(nomeBlocoTxt, "w");
        if (!blocoBin || !blocoTxt) {
            printf("Erro ao criar o arquivo do bloco %d!\n", contadorBlocos);
            break;
        }

        fwrite(dados, sizeof(int), lidos, blocoBin);
        for (size_t i = 0; i < lidos; i++)
            fprintf(blocoTxt, "%d\n", dados[i]);

        fclose(blocoBin);
        fclose(blocoTxt);

        printf("Bloco %d ordenado e salvo em '%s' e '%s'!\n", contadorBlocos, nomeBlocoBin, nomeBlocoTxt);
    }

    free(dados);
    fclose(arquivo);

    fimOrdenacao = clock();

    printf("\nTotal de blocos criados: %d\n", contadorBlocos);
    printf("Etapas de geração e ordenação concluídas com sucesso!\n");

    // ============ Etapa 3: Fusão externa ============
    fusaoExterna(contadorBlocos);

    fimTotal = clock();

    // Cálculo dos tempos
    double tempoGeracao = (double)(fimGeracao - inicioGeracao) / CLOCKS_PER_SEC;
    double tempoOrdenacao = (double)(fimOrdenacao - inicioOrdenacao) / CLOCKS_PER_SEC;
    double tempoTotal = (double)(fimTotal - inicioTotal) / CLOCKS_PER_SEC;

    // ============ RELATÓRIO FINAL ============
    FILE *relatorio = fopen("relatorio_execucao.txt", "a");
    if (relatorio != NULL) {
        time_t agora = time(NULL);
        struct tm *info = localtime(&agora);

        fprintf(relatorio, "=========================================\n");
        fprintf(relatorio, " Relatório de Execução - %02d/%02d/%d %02d:%02d:%02d\n",
                info->tm_mday, info->tm_mon + 1, info->tm_year + 1900,
                info->tm_hour, info->tm_min, info->tm_sec);
        fprintf(relatorio, "-----------------------------------------\n");
        fprintf(relatorio, "Tamanho total de dados: %.2f GB\n", tamanhoGB);
        fprintf(relatorio, "Quantidade de blocos: %d\n", contadorBlocos);
        fprintf(relatorio, "Tempo de geração do arquivo: %.2f segundos\n", tempoGeracao);
        fprintf(relatorio, "Tempo de ordenação: %.2f segundos\n", tempoOrdenacao);
        fprintf(relatorio, "Tempo total: %.2f segundos\n", tempoTotal);
        fprintf(relatorio, "=========================================\n\n");
        fclose(relatorio);
        printf("\nRelatório salvo em 'relatorio_execucao.txt'.\n");
    } else {
        printf("Erro ao criar o relatório.\n");
    }

    return 0;
}
