/*
 * 05_conta_texto.l — Conta caracteres, linhas e palavras (estilo "wc").
 *
 * Variaveis (bloco %{ %}, copiado como codigo C):
 *   car               -> total de caracteres
 *   lin               -> total de linhas
 *   pal               -> total de palavras
 *   ultima_foi_quebra -> flag: 1 se o ultimo caractere lido foi um "\n",
 *                        0 caso contrario. Usada para detectar se a
 *                        ultima linha do arquivo terminou sem quebra.
 *
 * Regras (a ORDEM é importante: o Flex tenta a correspondencia mais
 * longa possivel e, em caso de empate, usa a regra que aparece
 * primeiro no arquivo):
 *
 *   \n          -> conta como 1 caractere E fecha 1 linha (car++, lin++)
 *   [^ \t\n]+   -> uma "palavra": sequencia de 1+ caracteres que NAO
 *                  sejam espaco, tab ou quebra de linha. Conta 1 palavra
 *                  e soma ao total de caracteres o tamanho da palavra
 *                  (yyleng = numero de caracteres que casaram com a
 *                  regra atual).
 *   .           -> qualquer outro caractere isolado (na pratica, os
 *                  espacos e tabs que sobraram, um a um) conta como
 *                  1 caractere.
 *   <<EOF>>     -> regra especial do Flex, disparada uma unica vez ao
 *                  chegar no fim do arquivo. Se o ultimo caractere lido
 *                  NAO foi uma quebra de linha (ultima_foi_quebra == 0),
 *                  significa que a ultima linha ficou "pendurada", sem
 *                  \n no final — mesmo assim, ela conta como 1 linha.
 *                  Sem esta regra, arquivos que nao terminam com \n
 *                  ficariam com 1 linha a menos do que o esperado.
 *
 * Compilar:  flex -o 05_conta_texto.c 05_conta_texto.l && gcc 05_conta_texto.c -o 05_conta_texto -lfl
 * Executar:  ./05_conta_texto < teste.txt
 */
%{
int car = 0;
int lin = 0;
int pal = 0;
int ultima_foi_quebra = 1; /* comeca em 1: arquivo vazio tem 0 linhas */
%}

%%
\n        { car++; lin++; ultima_foi_quebra = 1; }
[^ \t\n]+ { pal++; car += yyleng; ultima_foi_quebra = 0; }
.         { car++; ultima_foi_quebra = 0; }

<<EOF>>   {
              if (!ultima_foi_quebra) lin++;
              yyterminate();
          }
%%

int yywrap(){
    return 1;
}

int main (){
    yylex();
    printf("Caracteres = %d, linhas = %d, palavras = %d\n", car, lin, pal);
    return 0;
}
