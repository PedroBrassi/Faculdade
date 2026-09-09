#include <stdio.h>
int main () {
    int q = 0;
    int delta[5][2] =
        {1,2,
         3,4,
         3,4,
         3,4,
         4,4};
    char *entrada = "aaab";
    for (int i = 0; entrada[i]; i++) {
        q = delta[q][entrada[i] - 'a'];
        printf("q = %d\n", q);
    }
    if (q == 1 || q == 2 || q == 3)
        puts("aceita");
    else
        puts("rejeita");
}