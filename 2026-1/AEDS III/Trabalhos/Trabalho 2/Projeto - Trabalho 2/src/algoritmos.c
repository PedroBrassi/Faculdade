#include "algoritmos.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
Participantes do grupo
Nome: Isabela M. Andrade        Matrícula: 2025.1.08.011
Nome: Isadora R. Grandeaux      Matrícula: 2025.1.08.012
Nome: Júlia A. da Rocha         Matrícula: 2025.1.08.015
Nome: Otavio de Oliveira        Matrícula: 2025.1.08.034
Nome: Pedro Brassi Luccas       Matrícula: 2022.2.08.009
*/

/*
 Função de pontuação entre duas bases.

 Regras:
 - A-T e T-A → +2 (pareamento correto)
 - C-G e G-C → +2 (pareamento correto)
 - Base com gap '-' → -2 (penalidade por lacuna)
 - Qualquer outro caso → -1 (mismatch)

 Observação:
 O gap não existe originalmente na string, mas pode ser inserido
 durante o alinhamento para melhorar a pontuação global.
*/

#define GAP -2

int score(char a, char b) {
    // Pares válidos (pareamento correto do DNA)
    if ((a == 'A' && b == 'T') || (a == 'T' && b == 'A') ||
        (a == 'C' && b == 'G') || (a == 'G' && b == 'C'))
        return 2;

    // Caso exista gap (lacuna)
    if ((a == 'A' && b == '-') || (a == '-' && b == 'A') ||
        (a == 'T' && b == '-') || (a == '-' && b == 'T') ||
        (a == 'C' && b == '-') || (a == '-' && b == 'C') ||
        (a == 'G' && b == '-') || (a == '-' && b == 'G'))
        return GAP;

    // Caso contrário, é mismatch (bases diferentes não complementares)
    return -1;
}

/*
 Algoritmo Guloso:

 Ideia geral:
 - Percorre as duas strings simultaneamente
 - Em cada passo, toma uma decisão LOCAL ótima, escolhendo a melhor entre:
     1) Alinhar duas bases (match ou mismatch)
     2) Inserir gap em s1
     3) Inserir gap em s2

 Características:
 - Gap tem custo -2 (penalidade maior que mismatch -1)
 - Por isso, o algoritmo tende a evitar gaps
 - Não considera decisões futuras → pode gerar solução subótima

 Complexidade:
 - Tempo: O(n + m)
 - Espaço: O(1)
*/

long int guloso(char *s1, char *s2) {

    int i = 0, j = 0;          // Índices para percorrer as strings
    long int pontuacao = 0;    // Acumulador da pontuação total

    int n = strlen(s1);
    int m = strlen(s2);

    // Continua até "consumir" ambas as strings
    while (i < n || j < m) {

        // Inicializa com valores muito baixos para evitar escolhas incorretas
        int match = -9999;
        int gap_s1 = -9999; // gap em s2 (avança apenas em s1)
        int gap_s2 = -9999; // gap em s1 (avança apenas em s2)

        // Caso seja possível alinhar duas bases (ambas ainda têm caracteres)
        if (i < n && j < m) {
            match = score(s1[i], s2[j]);
        }

        // Inserir gap em s2 → consome caractere de s1
        if (i < n) {
            gap_s1 = GAP;
        }

        // Inserir gap em s1 → consome caractere de s2
        if (j < m) {
            gap_s2 = GAP;
        }

        /*
         Escolha gulosa:
         Seleciona a melhor opção LOCAL (maior pontuação imediata),
         sem considerar impactos futuros.
        */
        if (match >= gap_s1 && match >= gap_s2) {
            pontuacao += match;
            i++;
            j++;
        }
        else if (gap_s1 >= gap_s2) {
            pontuacao += gap_s1;
            i++;
        }
        else {
            pontuacao += gap_s2;
            j++;
        }
    }

    return pontuacao;
}


/* Algoritmo de Programação Dinâmica:
“Programação dinâmica é uma técnica que resolve problemas complexos dividindo-os em subproblemas menores, 
armazenando suas soluções e reutilizando-as, garantindo eficiência e optimalidade.”

Ideia geral:
- Subprogramas sobrepostos (o mesmo cálculo aparece várias vezes)
- Subestrutua Ótima (pode ser encontrada a partir de soluções ótima menores)


    dp[0][0] = 0;
    diagonal = dp[i-1][j-1] + score (s1[i-1],s2[j-1]);
    cima = dp [i-1][j] - 2;
    esquerda = dp[i][j-1] - 2;
*/

// Retorna o maior valor entre três números
long int maiorTres(long int a, long int b, long int c){
    long int maior = a; // assume inicialmente que 'a' é o maior

    // compara com 'b'
    if (b > maior) maior = b;

    // compara com 'c'
    if (c > maior) maior = c;

    return maior; // retorna o maior dos três
}

// Função principal de Programação Dinâmica (alinhamento global)
long int programacao_dinamica(char *s1, char *s2) {

    // n = tamanho da string s1 (linhas da matriz)
    int n = strlen(s1);

    // m = tamanho da string s2 (colunas da matriz)
    int m = strlen(s2);

    // Alocação dinâmica da matriz dp (evita estouro de stack)
    // dp[i][j] = melhor pontuação para alinhar s1[0..i-1] com s2[0..j-1]
    long int **dp = malloc((n+1) * sizeof(long int*));
    for (int i = 0; i <= n; i++) {
        dp[i] = malloc((m+1) * sizeof(long int));
    }

    // Caso base: alinhar duas strings vazias
    dp[0][0] = 0;

    // Preenche primeira coluna (s2 vazia → só gaps em s2)
    // acumulando penalidade de GAP
    for (int i = 1; i <= n; i++)
        dp[i][0] = dp[i-1][0] + GAP;

    // Preenche primeira linha (s1 vazia → só gaps em s1)
    for (int j = 1; j <= m; j++)
        dp[0][j] = dp[0][j-1] + GAP;

    // Preenchimento da matriz
    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= m; j++){

            // ↖ Diagonal: alinhar s1[i-1] com s2[j-1]
            long int diagonal = dp[i-1][j-1] + score(s1[i-1], s2[j-1]);

            // ↑ Cima: gap em s2 (consome caractere de s1)
            long int cima = dp[i-1][j] + GAP;

            // ← Esquerda: gap em s1 (consome caractere de s2)
            long int esquerda = dp[i][j-1] + GAP;

            // escolhe a melhor opção (ótima local baseada em subproblemas)
            dp[i][j] = maiorTres(diagonal, cima, esquerda);
        }
    }

    // Resultado final: canto inferior direito da matriz
    long int pontuacao = dp[n][m];

    // Liberação da memória alocada
    for (int i = 0; i <= n; i++) {
        free(dp[i]);
    }
    free(dp);
    
    return pontuacao; // retorna a melhor pontuação possível
}