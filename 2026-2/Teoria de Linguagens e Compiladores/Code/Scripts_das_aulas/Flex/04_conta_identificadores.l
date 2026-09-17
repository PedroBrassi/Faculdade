/*
 * 04_conta_identificadores.l — Conta quantos identificadores existem no texto.
 *
 * Definicoes (macros de regex reaproveitadas nas regras abaixo):
 *   digit  -> um dígito (0-9)
 *   letter -> uma letra, maiuscula ou minuscula (A-Z ou a-z)
 *
 * Regras:
 *   {letter}({letter}|{digit})*  -> "identificador": comeca com letra,
 *                                    seguida de zero ou mais letras/digitos.
 *                                    ECHO imprime o identificador encontrado
 *                                    e count++ soma 1 ao contador.
 *   \n                           -> sem acao (regra "muda"): a quebra de
 *                                    linha e apenas consumida, sem nenhum
 *                                    codigo executado (nao imprime nada).
 *
 * Qualquer caractere que NAO seja letra/digito e que NAO seja \n
 * (espacos, por exemplo) nao casa com nenhuma regra explicita e por
 * isso cai na regra padrao do Flex, que apenas o copia para a saida.
 * E por isso que os espacos entre as palavras aparecem na saida.
 *
 * Compilar:  flex -o 04_conta_identificadores.c 04_conta_identificadores.l && gcc 04_conta_identificadores.c -o 04_conta_identificadores -lfl
 * Executar:  ./04_conta_identificadores < teste.txt
 */

digit [0-9]
letter [A-Za-z]

%{
int count = 0;
%}

%%
{letter}({letter}|{digit})* { ECHO; count++;}
\n
%%

int yywrap(){
    return 1;
}

int main (void) {
    yylex();
    printf("Quantidade de identificadores = %d\n", count);
    return 0;
}
