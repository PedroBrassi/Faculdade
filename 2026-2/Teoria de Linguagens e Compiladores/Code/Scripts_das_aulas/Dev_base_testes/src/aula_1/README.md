# Aula 1: Análise Léxica com Flex

## Estrutura de um arquivo `.l`

Um arquivo fonte do Flex é dividido em **três seções** separadas por `%%`:

```c
%{
    /* 1. Código C copiado diretamente para o início do arquivo gerado (#include, variáveis globais) */
%}

/* Definições: apelidos (macros) para expressões regulares */

%%

/* 2. Regras:   padrão   { ação em C } */

%%

/* 3. Código C copiado para o final do arquivo gerado (funções auxiliares, main) */
```

### Como o `yylex()` funciona?

O Flex compila o arquivo `.l` em um código C contendo a função `yylex()`. A cada chamada de `yylex()`, o analisador lê caracteres da entrada até casar com algum padrão e executa a respectiva ação:

* **Com `return`:** `yylex()` interrompe a leitura e devolve o valor especificado (geralmente o código do *token*).
* **Sem `return`:** `yylex()` continua lendo a entrada automaticamente — é exatamente assim que espaços em branco e comentários são ignorados.
* **Fim de arquivo (`EOF`):** `yylex()` retorna `0`.

Dentro de qualquer ação em C, você tem acesso imediato a duas variáveis globais principais:

* `yytext`: ponteiro (`char*`) contendo o texto exato que casou com a regra (ex.: `"contador"`).
* `yyleng`: inteiro (`int`) indicando o tamanho de `yytext`.

---

## Exemplo Prático (Esquenta)

> **Nota:** Este é apenas um exemplo didático inicial, ainda não é a linguagem **Simples**.

Crie um arquivo chamado `exemplo.l` com o conteúdo abaixo:

```c
%{
#include <stdio.h>
%}

digito [0-9]
letra  [a-zA-Z]

%%
"ola"         { printf("SAUDACAO\n"); }
{digito}+     { printf("NUMERO(%s)\n", yytext); }
{letra}+      { printf("PALAVRA(%s)\n", yytext); }
[ \t\n]+      { /* ignora espaços, tabulações e quebras de linha */ }
.             { printf("ERRO(%s)\n", yytext); }
%%

int yywrap() { return 1; }

int main() {
    yylex();
    return 0;
}
```

### Compilando e testando

No terminal, gere o código C, compile com o `gcc` e passe uma entrada de teste:

```bash
flex -o exemplo.c exemplo.l
gcc exemplo.c -o exemplo
echo "ola 123 abc olaaa x9 ?" | ./exemplo
```

### Saída esperada

```text
SAUDACAO
NUMERO(123)
PALAVRA(abc)
PALAVRA(olaaa)
PALAVRA(x)
NUMERO(9)
ERRO(?)
```

---

## As Regras de Desempate do Flex

Observe a saída gerada com atenção. Ela ilustra os **dois critérios fundamentais** que o Flex usa quando uma entrada pode casar com mais de uma regra:

1. **Casamento mais longo vence (*Maximal Munch*):** 
   A palavra `olaaa` casa tanto com `"ola"` (3 caracteres) quanto com `{letra}+` (5 caracteres). Como o Flex sempre prioriza o maior casamento possível, ela é classificada como `PALAVRA`.
2. **Empate de tamanho — vence quem aparece primeiro:** 
   A palavra `ola` casa com `"ola"` e com `{letra}+`, ambos com 3 caracteres. Nesse caso, vence a regra declarada mais acima no arquivo (`"ola"`), resultando em `SAUDACAO`.