#ifndef LEITURA_H
#define LEITURA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Struct Jogador */
typedef struct {
    char nome[100];
    char posicao[50];
    char naturalidade[100];
    char time[100];
    int idade;
} Jogador;

/* Trim de fim de string (remove \r e \n e espaços finais) */
static void trim_end(char *s) {
    int t = (int)strlen(s);
    while (t > 0 && (s[t-1] == '\n' || s[t-1] == '\r' || s[t-1] == ' ' || s[t-1] == '\t'))
        s[--t] = '\0';
}

/* Conta linhas válidas (desconsidera cabeçalho) */
static int contarLinhasCSV(const char *nome) {
    FILE *f = fopen(nome, "r");
    if (!f) {
        fprintf(stderr, "Erro ao abrir %s\n", nome);
        return -1;
    }
    int linhas = 0;
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), f)) {
        linhas++;
    }
    fclose(f);
    if (linhas <= 1) return 0;
    return linhas - 1; // ignora a linha de cabeçalho
}

/* Imprime lista no formato CSV (mesmo formato de entrada) */
static void imprimirJogadoresCSV(Jogador *vet, int n) {
    // cabeçalho igual ao do CSV de entrada
    printf("Nome,Posicao,Naturalidade,Clube,Idade\n");
    for (int i = 0; i < n; i++) {
        printf("%s,%s,%s,%s,%d\n",
               vet[i].nome,
               vet[i].posicao,
               vet[i].naturalidade,
               vet[i].time,
               vet[i].idade);
    }
}

/* Lê o arquivo CSV e preenche vetor de Jogador
   Retorna ponteiro para array alocado e escreve qtd e atualiza memoria (em bytes)
*/
static Jogador *lerArquivoCSV(const char *nome, int *qtd, long long *memoria) {
    if (!qtd || !memoria) return NULL;
    *qtd = 0;

    int linhas = contarLinhasCSV(nome);
    if (linhas < 0) exit(1);
    *qtd = linhas;

    // contabiliza memória para o vetor de structs
    *memoria += (long long)(linhas) * sizeof(Jogador);

    Jogador *lista = malloc((size_t)linhas * sizeof(Jogador));
    if (!lista) {
        fprintf(stderr, "Erro de memória ao alocar lista\n");
        exit(1);
    }

    FILE *f = fopen(nome, "r");
    if (!f) {
        fprintf(stderr, "Erro ao abrir %s\n", nome);
        free(lista);
        exit(1);
    }

    char linha[1024];
    // lê e descarta cabeçalho
    if (!fgets(linha, sizeof(linha), f)) {
        fprintf(stderr, "Arquivo vazio ou problema ao ler cabeçalho\n");
        fclose(f);
        free(lista);
        exit(1);
    }

    int idx = 0;
    while (fgets(linha, sizeof(linha), f) && idx < linhas) {
        trim_end(linha);
        if (linha[0] == '\0') continue;

        // usa strtok_r com delimitador ','
        char *saveptr = NULL;
        char *token = strtok_r(linha, ",", &saveptr);
        if (!token) { strcpy(lista[idx].nome, ""); }
        else { // copia nome
            // remover espaços iniciais
            while (*token == ' ') token++;
            strncpy(lista[idx].nome, token, sizeof(lista[idx].nome)-1);
            lista[idx].nome[sizeof(lista[idx].nome)-1] = '\0';
        }

        token = strtok_r(NULL, ",", &saveptr);
        if (!token) lista[idx].posicao[0] = '\0';
        else {
            while (*token == ' ') token++;
            strncpy(lista[idx].posicao, token, sizeof(lista[idx].posicao)-1);
            lista[idx].posicao[sizeof(lista[idx].posicao)-1] = '\0';
        }

        token = strtok_r(NULL, ",", &saveptr);
        if (!token) lista[idx].naturalidade[0] = '\0';
        else {
            while (*token == ' ') token++;
            strncpy(lista[idx].naturalidade, token, sizeof(lista[idx].naturalidade)-1);
            lista[idx].naturalidade[sizeof(lista[idx].naturalidade)-1] = '\0';
        }

        token = strtok_r(NULL, ",", &saveptr);
        if (!token) lista[idx].time[0] = '\0';
        else {
            while (*token == ' ') token++;
            strncpy(lista[idx].time, token, sizeof(lista[idx].time)-1);
            lista[idx].time[sizeof(lista[idx].time)-1] = '\0';
        }

        token = strtok_r(NULL, ",", &saveptr);
        if (!token) lista[idx].idade = 0;
        else {
            while (*token == ' ') token++;
            lista[idx].idade = atoi(token);
        }

        // trim de campos (remove \r\n e espaços finais)
        trim_end(lista[idx].nome);
        trim_end(lista[idx].posicao);
        trim_end(lista[idx].naturalidade);
        trim_end(lista[idx].time);

        idx++;
    }

    fclose(f);
    // caso linhas lidas sejam menos que contarLinhasCSV, ajusta qtd e mem
    if (idx != linhas) {
        *qtd = idx;
    }

    return lista;
}

#endif 
