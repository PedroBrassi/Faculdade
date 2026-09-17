/*
 * 03_numera_linhas.l — Numera as linhas de um arquivo (estilo "cat -n" ou "nl").
 *
 * A variavel "conta" (declarada no bloco %{ %}, que e copiado
 * literalmente para o codigo C gerado) guarda o numero da linha atual.
 *
 * Regras:
 *   ^(.*)\n  -> a cada linha completa (do inicio "^" até a quebra de
 *               linha "\n"), incrementa "conta" e imprime o numero da
 *               linha seguido do conteudo dela (yytext ja inclui o \n).
 *   ^(.*)    -> cobre o caso da ULTIMA linha do arquivo quando ela nao
 *               termina com "\n" (situacao comum: editor nao grava uma
 *               quebra de linha final). Sem esta segunda regra, essa
 *               ultima linha cairia na regra padrao do Flex e seria
 *               apenas copiada para a saida, sem numero. Como esta
 *               regra nao inclui o "\n" em yytext (porque ele nao
 *               existe nessa linha), a acao imprime o "\n" manualmente
 *               no final, so para manter a formatacao da saida.
 *
 * ATENCAO — pegadinha que ainda existe neste exemplo:
 *
 *  Se o arquivo de entrada usar quebra de linha estilo Windows
 *  (\r\n em vez de \n), o "\r" fica incluido dentro do "(.*)" —
 *  ou seja, o \r e impresso junto com o texto da linha, o que pode
 *  causar um efeito visual estranho no terminal (o cursor volta
 *  para o inicio da linha antes do "\n" real). Isso nao afeta a
 *  numeracao das linhas, so a exibicao do conteudo de uma linha
 *  especifica.
 *
 * Este exemplo le de um ARQUIVO passado por linha de comando
 * (argv[1]), diferente dos outros exemplos desta pasta, que leem
 * da entrada padrao (stdin).
 *
 * Compilar:  flex -o 03_numera_linhas.c 03_numera_linhas.l && gcc 03_numera_linhas.c -o 03_numera_linhas -lfl
 * Executar:  ./03_numera_linhas teste.txt
 */
%{
int conta = 0;
%}

%%
^(.*)\n    printf("%4d: %s", ++conta, yytext);
^(.*)      printf("%4d: %s\n", ++conta, yytext);
%%

int yywrap (){
    return 1;
}

int main (int argc, char *argv[]) {
    yyin = fopen (argv[1], "rt");
    yylex();
    fclose(yyin);
    return 0;
}
