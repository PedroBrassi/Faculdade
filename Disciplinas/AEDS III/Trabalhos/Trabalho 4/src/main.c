#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../include/partition.h"
#include "../include/simulatedAnnealing.h"
#include "../include/genetico.h"
#include "../include/guloso.h"

// --- DIRETIVAS DE COMPILAÇÃO CONDICIONAL ---
// Permite que o programa seja multiplataforma, tratando a criação de pastas
// de forma nativa tanto no ecossistema Windows quanto em sistemas POSIX (Linux/macOS).
#ifdef _WIN32
    #include <direct.h>
    #define CRIAR_PASTA_RESULTADOS(pasta) _mkdir(pasta)
#else
    #define CRIAR_PASTA_RESULTADOS(pasta) mkdir(pasta, 0777) // 0777 concede permissões totais de leitura/escrita
#endif

int main() {
    // Registra a contagem de ciclos de CPU no exato instante em que o programa inicia.
    clock_t global_inicio = clock();

    // Define uma semente fixa para o gerador de números pseudoaleatórios.
    // Garante a reproducibilidade científica dos testes (mesmos resultados em qualquer máquina).
    srand(42);
    
    // Cria o diretório de saída caso ele ainda não exista no sistema.
    CRIAR_PASTA_RESULTADOS("resultados");

    // Vetor com as diferentes granularidades de instâncias (escalabilidade do problema)
    int tamanhos[] = {100, 200, 500, 1000, 2500, 5000, 10000};
    int qte_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]); // Truque estatístico para obter o tamanho do array
    const int RUNS = 5; // Número de execuções independentes por instância (estabilidade estatística)

    // --- ABERTURA DOS ARQUIVOS DE LOG E CONSOLIDAÇÃO ---
    FILE *arq_puro   = fopen("resultados/PartitionPuro.txt", "w");
    FILE *arq_sa     = fopen("resultados/SimulatedAnnealing.txt", "w");
    FILE *arq_ga     = fopen("resultados/Genetico.txt", "w");
    FILE *arq_guloso = fopen("resultados/Guloso.txt", "w");
    FILE *arq_medio  = fopen("resultados/resultadoMedio.txt", "w");
    FILE *arq_tabela = fopen("resultados/resultadosTabela.txt", "w");

    // Tratamento de erro robusto: se qualquer arquivo falhar ao abrir, fecha todos os outros
    // que obtiveram sucesso para evitar vazamento de descritores do sistema operacional (Resource Leak).
    if (arq_puro == NULL || arq_sa == NULL || arq_ga == NULL || arq_guloso == NULL || arq_medio == NULL || arq_tabela == NULL) {
        printf("Erro critico ao criar os arquivos na pasta 'resultados/'.\n");
        if (arq_puro) fclose(arq_puro);
        if (arq_sa) fclose(arq_sa);
        if (arq_ga) fclose(arq_ga);
        if (arq_guloso) fclose(arq_guloso);
        if (arq_medio) fclose(arq_medio);
        if (arq_tabela) fclose(arq_tabela);
        return 1;
    }

    // --- FORMATAÇÃO E GRAVAÇÃO DOS CABEÇALHOS ---
    char cabecalho[] = "%-25s | %-5s | %-12s | %-15s | %-15s\n-----------------------------------------------------------------------------------------\n";
    
    fprintf(arq_puro, "=========================================================================================\n"
                      "                  LOG DE EXECUCOES DETALHADAS - PARTITION PURO (BASE)                     \n"
                      "=========================================================================================\n");
    fprintf(arq_puro, cabecalho, "Instancia", "Run", "Tamanho (N)", "Fitness", "Tempo");

    fprintf(arq_sa, "=========================================================================================\n"
                    "                  LOG DE EXECUCOES DETALHADAS - SIMULATED ANNEALING                       \n"
                    "=========================================================================================\n");
    fprintf(arq_sa, cabecalho, "Instancia", "Run", "Tamanho (N)", "Fitness", "Tempo");

    fprintf(arq_ga, "=========================================================================================\n"
                    "                  LOG DE EXECUCOES DETALHADAS - ALGORITMO GENETICO                        \n"
                    "=========================================================================================\n");
    fprintf(arq_ga, cabecalho, "Instancia", "Run", "Tamanho (N)", "Fitness", "Tempo");

    fprintf(arq_guloso, "=========================================================================================\n"
                        "                  LOG DE EXECUCOES DETALHADAS - ALGORITMO GULOSO                          \n"
                        "=========================================================================================\n");
    fprintf(arq_guloso, cabecalho, "Instancia", "Run", "Tamanho (N)", "Fitness", "Tempo");

    // Cabeçalho do arquivo compilado amigável para leitura humana
    fprintf(arq_medio, "========================================================================================\n"
                       "                  RESULTADOS MEDIOS COMPILADOS POR TAMANHO                              \n"
                       "========================================================================================\n"
                       "%-12s | %-20s | %-20s | %-20s\n----------------------------------------------------------------------------------------\n", 
                       "Tamanho (N)", "Algoritmo", "Fitness Medio", "Tempo Medio (s)");

    // Cabeçalho do arquivo estruturado (CSV) ideal para importação em scripts de Python (Pandas/Matplotlib) ou Excel
    fprintf(arq_tabela, "Algoritmo,Instancia,Tamanho,FitnessMedio,TempoMedio(s)\n");

    printf("====================================================================================\n");
    printf("    Iniciando Experimentos (%d runs por instancia - Usando seed = 42)\n", RUNS);
    printf("====================================================================================\n\n");

    // --- LOOP PRINCIPAL 1: ITERA SOBRE OS TAMANHOS (N) ---
    for (int i = 0; i < qte_tamanhos; i++) {
        int n = tamanhos[i];
        
        // Acumuladores de médias gerais de todas as 5 variações do tamanho N corrente
        double soma_fit_puro = 0.0, soma_temp_puro = 0.0;
        double soma_fit_sa   = 0.0, soma_temp_sa   = 0.0;
        double soma_fit_ga   = 0.0, soma_temp_ga   = 0.0;
        double soma_fit_guloso = 0.0, soma_temp_guloso = 0.0;

        // Acumuladores locais para monitorar o tempo total gasto pelo bloco de algoritmos para o tamanho N
        double bloco_temp_puro = 0.0, bloco_temp_sa = 0.0, bloco_temp_ga = 0.0, bloco_temp_guloso = 0.0;

        // --- LOOP PRINCIPAL 2: ITERA SOBRE OS TIPOS DE VARIABILIDADE (1 a 5) ---
        for (int tipo = 1; tipo <= 5; tipo++) {
            char caminho_arquivo[150];
            sprintf(caminho_arquivo, "instancias/instancia_%d_%d.txt", n, tipo);
            char nome_exibicao[50];
            sprintf(nome_exibicao, "instancia_%d_%d.txt", n, tipo);

            // Carrega os dados numéricos do arquivo de teste correspondente
            Instancia *inst = ler_instancia(caminho_arquivo);
            if (inst == NULL) return 1; // Interrompe o programa caso o arquivo esteja corrompido ou ausente

            // Acumuladores locais de fitness e tempo para as 5 runs desta instância específica
            double r_fit_puro = 0.0, r_temp_puro = 0.0;
            double r_fit_sa   = 0.0, r_temp_sa   = 0.0;
            double r_fit_ga   = 0.0, r_temp_ga   = 0.0;
            double r_fit_guloso = 0.0, r_temp_guloso = 0.0;

            // ==========================================
            // 1. BATERIA DE EXECUÇÃO: PARTITION PURO
            // ==========================================
            for (int r = 1; r <= RUNS; r++) {
                clock_t inicio = clock();
                Solucao sol = { .config = NULL };
                inicializar_solucao_padrao(&sol, inst); // Alocação inicial determinística
                avaliar_solucao(&sol, inst);
                clock_t fim = clock();
                double t = (double)(fim - inicio) / CLOCKS_PER_SEC; // Conversão dos ciclos para segundos

                fprintf(arq_puro, "%-25s | #%-3d | N=%-10d | %-15lld | %.6f s\n", nome_exibicao, r, n, sol.fitness, t);
                r_fit_puro += (double)sol.fitness; r_temp_puro += t; bloco_temp_puro += t;
                liberar_solucao(&sol); // Prevenção ativa de estouro de memória
            }
            fprintf(arq_puro, "-----------------------------------------------------------------------------------------\n");

            // ==========================================
            // 2. BATERIA DE EXECUÇÃO: SIMULATED ANNEALING
            // ==========================================
            for (int r = 1; r <= RUNS; r++) {
                clock_t inicio = clock();
                Solucao sol = executar_simulated_annealing(inst);
                clock_t fim = clock();
                double t = (double)(fim - inicio) / CLOCKS_PER_SEC;

                fprintf(arq_sa, "%-25s | #%-3d | N=%-10d | %-15lld | %.6f s\n", nome_exibicao, r, n, sol.fitness, t);
                r_fit_sa += (double)sol.fitness; r_temp_sa += t; bloco_temp_sa += t;
                liberar_solucao(&sol);
            }
            fprintf(arq_sa, "-----------------------------------------------------------------------------------------\n");

            // ==========================================
            // 3. BATERIA DE EXECUÇÃO: ALGORITMO GENÉTICO
            // ==========================================
            for (int r = 1; r <= RUNS; r++) {
                clock_t inicio = clock();
                Solucao sol = executar_algoritmo_genetico(inst);
                clock_t fim = clock();
                double t = (double)(fim - inicio) / CLOCKS_PER_SEC;

                fprintf(arq_ga, "%-25s | #%-3d | N=%-10d | %-15lld | %.6f s\n", nome_exibicao, r, n, sol.fitness, t);
                r_fit_ga += (double)sol.fitness; r_temp_ga += t; bloco_temp_ga += t;
                liberar_solucao(&sol);
            }
            fprintf(arq_ga, "-----------------------------------------------------------------------------------------\n");

            // ==========================================
            // 4. BATERIA DE EXECUÇÃO: ALGORITMO GULOSO
            // ==========================================
            for (int r = 1; r <= RUNS; r++) {
                clock_t inicio = clock();
                Solucao sol = executar_algoritmo_guloso(inst);
                clock_t fim = clock();
                double t = (double)(fim - inicio) / CLOCKS_PER_SEC;

                fprintf(arq_guloso, "%-25s | #%-3d | N=%-10d | %-15lld | %.6f s\n", nome_exibicao, r, n, sol.fitness, t);
                r_fit_guloso += (double)sol.fitness; r_temp_guloso += t; bloco_temp_guloso += t;
                liberar_solucao(&sol);
            }
            fprintf(arq_guloso, "-----------------------------------------------------------------------------------------\n");

            // --- PROCESSAMENTO ACUMULATIVO DAS MÉDIAS (Desta Instância) ---
            double m_fit_puro = r_fit_puro / RUNS, m_temp_puro = r_temp_puro / RUNS;
            double m_fit_sa   = r_fit_sa   / RUNS, m_temp_sa   = r_temp_sa   / RUNS;
            double m_fit_ga   = r_fit_ga   / RUNS, m_temp_ga   = r_temp_ga   / RUNS;
            double m_fit_guloso = r_fit_guloso / RUNS, m_temp_guloso = r_temp_guloso / RUNS;

            // Gravação imediata dos resultados consolidados da instância no arquivo CSV
            fprintf(arq_tabela, "PartitionPuro,%s,%d,%.2f,%.6f\n", nome_exibicao, n, m_fit_puro, m_temp_puro);
            fprintf(arq_tabela, "SimulatedAnnealing,%s,%d,%.2f,%.6f\n", nome_exibicao, n, m_fit_sa, m_temp_sa);
            fprintf(arq_tabela, "Genetico,%s,%d,%.2f,%.6f\n", nome_exibicao, n, m_fit_ga, m_temp_ga);
            fprintf(arq_tabela, "Guloso,%s,%d,%.2f,%.6f\n", nome_exibicao, n, m_fit_guloso, m_temp_guloso);

            // Acumulação para o cálculo da média macro (média das médias dos 5 tipos de arquivos de tamanho N)
            soma_fit_puro += m_fit_puro; soma_temp_puro += m_temp_puro;
            soma_fit_sa   += m_fit_sa;   soma_temp_sa   += m_temp_sa;
            soma_fit_ga   += m_fit_ga;   soma_temp_ga   += m_temp_ga;
            soma_fit_guloso += m_fit_guloso; soma_temp_guloso += m_temp_guloso;

            // Desaloca os arrays dinâmicos da instância corrente para receber o próximo arquivo de teste limpo
            liberar_instancia(inst);
        }

        // --- EXIBIÇÃO EM TEMPO REAL (TERMINAL) ---
        // Exibe o progresso do pipeline de testes na tela. Útil para verificar que o programa 
        // não travou durante o processamento pesado das instâncias grandes (N=10000).
        printf("[PartitionPuro]      - Finalizado para %-5d - Tempo total: %.6f s\n", n, bloco_temp_puro);
        printf("[SimulatedAnnealing] - Finalizado para %-5d - Tempo total: %.6f s\n", n, bloco_temp_sa);
        printf("[Genetico]           - Finalizado para %-5d - Tempo total: %.6f s\n", n, bloco_temp_ga);
        printf("[Guloso]             - Finalizado para %-5d - Tempo total: %.6f s\n", n, bloco_temp_guloso);
        printf("----------------------------------------------------------------------------\n");
        fflush(stdout); // Força o esvaziamento do buffer do terminal (garante renderização síncrona na tela)

        // --- GRAVAÇÃO COMPILADA GERAL (Humana) ---
        // Registra a média final unificada das 25 execuções totais daquele bloco de tamanho N (5 tipos x 5 runs)
        fprintf(arq_medio, "%-12d | %-20s | %-20.2f | %-20.6f s\n", n, "PartitionPuro", soma_fit_puro / 5.0, soma_temp_puro / 5.0);
        fprintf(arq_medio, "%-12d | %-20s | %-20.2f | %-20.6f s\n", n, "SimulatedAnnealing", soma_fit_sa / 5.0, soma_temp_sa / 5.0);
        fprintf(arq_medio, "%-12d | %-20s | %-20.2f | %-20.6f s\n", n, "Genetico", soma_fit_ga / 5.0, soma_temp_ga / 5.0);
        fprintf(arq_medio, "%-12d | %-20s | %-20.2f | %-20.6f s\n", n, "Guloso", soma_fit_guloso / 5.0, soma_temp_guloso / 5.0);
        fprintf(arq_medio, "----------------------------------------------------------------------------------------\n");
    }

    // Fecha todos os manipuladores de arquivos abertos de forma limpa e segura
    fclose(arq_puro); fclose(arq_sa); fclose(arq_ga); fclose(arq_guloso); fclose(arq_medio); fclose(arq_tabela);

    // --- CÁLCULO E EXIBIÇÃO DO TEMPO GLOBAL ---
    // Mede os ciclos finais de CPU, calcula a diferença em relação ao início e encerra o pipeline geral.
    clock_t global_fim = clock();
    double global_tempo_total = (double)(global_fim - global_inicio) / CLOCKS_PER_SEC;

    printf("\n========================================================================\n");
    printf("Todos os experimentos concluidos com sucesso! Pasta 'resultados/' pronta.\n");
    printf("Tempo total global de execucao: %.6f s\n", global_tempo_total);
    printf("========================================================================\n");

    return 0;
}