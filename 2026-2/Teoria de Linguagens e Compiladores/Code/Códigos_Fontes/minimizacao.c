#include <stdio.h>
#include <stdbool.h>

// #define NSTATES 6
// #define NSYMBOLS 2
// int finals[NSTATES] = {1, 0, 1, 0, 1, 0};
// int afd[NSTATES][NSYMBOLS] =
//     {{1, 5},
//      {2, 4},
//      {3, 1},
//      {4, 2},
//      {1, 3},
//      {5, 5}};
// int marked[NSTATES][NSTATES];

// #define NSTATES 6
// #define NSYMBOLS 2
// int finals[NSTATES] = {0, 1, 1, 0, 0, 1};
// int afd[NSTATES][NSYMBOLS] =
//     {{1, 2},
//      {3, 4},
//      {4, 3},
//      {5, 5},
//      {5, 5},
//      {5, 5}};
// int marked[NSTATES][NSTATES];

// #define NSTATES 6
// #define NSYMBOLS 1
// int finals[NSTATES] = {0, 1, 0, 0, 1, 0};
// int afd[NSTATES][NSYMBOLS] =
//     {{1},
//      {2},
//      {3},
//      {4},
//      {5},
//      {0}};
// int marked[NSTATES][NSTATES];

#define NSTATES 6
#define NSYMBOLS 2
int finals[NSTATES] = {0, 0, 0, 1, 1, 1};
int afd[NSTATES][NSYMBOLS] =
    {{1, 2},
     {3, 4},
     {4, 3},
     {5, 5},
     {5, 5},
     {5, 5}};
int marked[NSTATES][NSTATES];

int grouped[NSTATES];

void show()
{
    printf("\n%2d\n", 0);
    for (int q = 1; q < NSTATES; q++)
    {
        for (int p = 0; p < q; p++)
            printf("%2c ", marked[p][q] ? 'x' : '.');
        printf("%2d \n", q);
    }
}

int main(void)
{
    bool changed;
    // Para AFD = (Q, Sigma, delta, s, F)
    // 1. Seja marked uma tabela com todos os pares
    //    NÃO ORDENADOS {p, q}, inicialmente desmarcados
    //    ou seja, {p, q} = {q, p}
    // 2. Marque {p, q} se p in F e q notin F, ou
    //    vice-versa
    for (int q = 1; q < NSTATES; q++)
        for (int p = 0; p < q; p++)
            if ((finals[p] && !finals[q]) ||
                (!finals[p] && finals[q]))
            {
                marked[p][q] = 1;
                marked[q][p] = 1;
            }
            else
            {
                marked[p][q] = 0;
                marked[q][p] = 0;
            }
    puts("\nInitialization");
    show();
    // 3. Repita o seguinte passo até que nenhuma
    //    mudança ocorra: se existe um par {p,q}
    //    tal que {d(p,a), d(q,a)} está marcado
    //    para algum a in Sigma, entao marque {p,q}
    int i = 0;
    do
    {
        changed = false;
        printf("\nIteration %d\n", ++i);
        for (int p = 0; p < NSTATES; p++)
            for (int q = p + 1; q < NSTATES; q++)
                if (!marked[p][q])
                {
                    printf("{%d,%d} is not marked\n", p, q);
                    for (int k = 0; k < NSYMBOLS; k++)
                    {
                        int dP = afd[p][k];
                        int dQ = afd[q][k];
                        printf("\t{d(%d,%d}, d(%d,%d)} = {%d, %d} ", p, k, q, k, dP, dQ);
                        if (marked[dP][dQ])
                        {
                            printf("is marked\n");
                            marked[p][q] = 1;
                            marked[q][p] = 1;
                            changed = true;
                        }
                        else
                        {
                            printf("is not marked\n");
                        }
                    }
                }
        show();
    } while (changed);
    // show();
    //  4. Ao final, p é equivalente a q sse
    //     {p, q} não está marcado
    for (int i = 0; i < NSTATES; i++)
    {
        grouped[i] = 0;
    }

    printf("\nEquivalent States\n");
    for (int p = 0; p < NSTATES; p++)
    {
        if (!grouped[p])
        {
            printf("{ %d ", p);
            grouped[p] = 1;
            for (int q = p + 1; q < NSTATES; q++)
                if (!marked[p][q]) {
                    printf("%d ", q);
                    grouped[q] = 1;
                }
            printf("}\n");
        }
    }
}