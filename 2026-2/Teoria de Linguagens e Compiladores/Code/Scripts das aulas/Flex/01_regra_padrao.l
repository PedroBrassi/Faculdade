/*
 * 01_regra_padrao.l — Exemplo minimo de um analisador Flex.
 *
 * Nao ha nenhuma regra entre os dois "%%" e nao ha secao de codigo
 * C no final (nao ha main() nem yywrap() definidos aqui).
 *
 * Como nenhuma regra e definida, o Flex usa sua REGRA PADRAO:
 * qualquer caractere de entrada que nao case com nenhuma regra e
 * copiado direto para a saida (efeito parecido com o comando "cat").
 *
 * Como o main() e o yywrap() nao existem neste arquivo, eles vem
 * da biblioteca do Flex ao linkar com "-lfl": ela fornece um main()
 * padrao (que so chama yylex()) e um yywrap() padrao (que retorna 1,
 * avisando "nao ha mais arquivos de entrada"). Por isso este exemplo
 * compila e funciona mesmo sem nenhuma linha de codigo C.
 *
 * Compilar:  flex -o 01_regra_padrao.c 01_regra_padrao.l && gcc 01_regra_padrao.c -o 01_regra_padrao -lfl
 * Executar:  ./01_regra_padrao < teste.txt   (ou digite algo e Ctrl+D)
 */
%%
