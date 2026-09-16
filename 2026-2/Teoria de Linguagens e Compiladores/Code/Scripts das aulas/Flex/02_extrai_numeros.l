/*
 * 02_extrai_numeros.l — Reconhece numeros e descarta o restante.
 *
 * Regras:
 *   [0-9]+  -> ECHO;   imprime o numero encontrado (ECHO imprime yytext,
 *                       ou seja, o proprio texto que casou com a regra)
 *   .       -> ;       qualquer outro caractere (exceto \n) e descartado
 *                       silenciosamente (acao vazia)
 *   \n      -> ;       a quebra de linha tambem e descartada
 *
 * Efeito pratico: a saida e a entrada com tudo que NAO for numero
 * removido, inclusive os espacos e as quebras de linha (por isso os
 * numeros aparecem todos colados, sem espaco, na saida).
 *
 * Compilar:  flex -o 02_extrai_numeros.c 02_extrai_numeros.l && gcc 02_extrai_numeros.c -o 02_extrai_numeros -lfl
 * Executar:  ./02_extrai_numeros < teste.txt
 */

%%
[0-9]+ ECHO;
.      ;
\n     ;
%%

int yywrap (void){
    return 1;
}

int main (void) {
    yylex();
    return 0;
}
