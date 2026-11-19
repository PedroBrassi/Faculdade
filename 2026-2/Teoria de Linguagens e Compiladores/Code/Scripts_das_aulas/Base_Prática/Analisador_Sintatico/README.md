# Analisador da linguagem Simples — Parte 2

Projeto em C que usa Flex e GNU Bison para validar a estrutura de programas
escritos na linguagem Simples. O arquivo informado é analisado até o fim;
erros são apresentados em `stderr`, com a linha e o símbolo encontrado.

O projeto **não é um interpretador**: `leia`, `escreva`, atribuições e controle
de fluxo são reconhecidos, mas não executados. Não há geração de código,
tabela de símbolos, verificação de variáveis declaradas ou de tipos.

## Requisitos

- GNU Make 4.3 ou superior (regras de geração agrupadas).
- GNU Bison 3.6 ou superior (diagnóstico sintático personalizado).
- Flex e compilador C com suporte a C11, como GCC.
- Ambiente com shell POSIX e `rm`, como Linux ou MSYS2 no Windows.

No Windows, use o terminal MSYS2 UCRT64, com GCC, Make, Bison e Flex no PATH.
O Makefile seleciona `simples.exe` quando `OS=Windows_NT`; nos demais ambientes
o executável se chama `simples`. A biblioteca `libfl` não é necessária.

## Compilar e usar

Na pasta do projeto:

```sh
make
make run
make run INPUT="outro programa.simples"
```

Também é possível executar diretamente:

```sh
./simples teste.simples       # Linux
./simples.exe teste.simples   # Windows/MSYS2
```

Um programa válido não produz mensagens e retorna código zero.
O arquivo é um argumento obrigatório; a entrada padrão não é usada.

```sh
make clean       # Remove executável, objetos e arquivos gerados
make limpa       # Mesmo efeito de clean
make -j4         # Compilação paralela
```

As ferramentas e opções podem ser substituídas pela linha de comando:
`make CC=gcc CFLAGS="-std=c11 -Wall -Wextra -g"`.
Após mudar opções de compilação, use `make clean` antes de recompilar.

## Estrutura

| Arquivo | Função |
| --- | --- |
| `lexico.l` | Tokens, espaços, contagem de linhas e erros léxicos |
| `sintatico.y` | Gramática, diagnósticos e função principal |
| `Makefile` | Geração, compilação, execução e limpeza |
| `teste.simples` | Exemplo de entrada válido |
| `README.md` | Funcionamento e instruções |

O processo de compilação gera `sintatico.c`, `sintatico.h`,
`sintatico.output` (relatório do Bison), `lexico.c`, os objetos e o executável.
Esses arquivos são reconstruídos a partir das fontes e não estão no ZIP final.

## Linguagem aceita

Estrutura geral:

```text
programa nome
    inteiro a b
    logico condicao
inicio
    leia a
    condicao <- NAO F E (a > 0)
    se condicao entao
        escreva a
    senao
        escreva 0
    fimse
    enquanto a > 0 faca
        a <- a - 1
    fimenquanto
fimprograma
```

- Declarações são opcionais, precedem `inicio` e usam `inteiro` ou `logico`
  seguidos de um ou mais identificadores separados por espaços.
- Identificadores começam com uma letra ASCII e continuam com letras ou
  dígitos. Não aceitam `_` nem acentos.
- Números são inteiros sem sinal, escritos com dígitos. Não há literais
  decimais, strings ou operador de menos unário; use `0 - 1` para uma
  expressão negativa.
- Booleanos: `V` e `F`. Operadores lógicos: `E`, `OU`, `NAO`.
- Comandos: `leia identificador`, `escreva expressao`,
  `identificador <- expressao`, `enquanto ... faca ... fimenquanto`,
  `se ... entao ... senao ... fimse`.
- `senao` é obrigatório em toda seleção. Os blocos podem estar vazios;
  seleções e repetições podem ser aninhadas.
- Expressões aceitam `+`, `-`, `*`, `div`, `>`, `<`, `=`, operadores lógicos
  e parênteses. Divisão se escreve `div`, não `/`.
- A linguagem distingue maiúsculas de minúsculas. Palavras-chave devem
  ser escritas exatamente como nos exemplos.
- Espaços, tabulações e quebras de linha LF/CRLF são ignorados. Quebras
  de linha não encerram comandos. Não há comentários, vírgulas ou
  ponto e vírgula na gramática.

Precedência, da menor para a maior: `E` e `OU` (mesmo nível), `=`,
`>` e `<`, `+` e `-`, `*` e `div`, `NAO` sobre um termo.
Os operadores binários associam à esquerda. Parênteses permitem alterar
o agrupamento. Comparações encadeadas são aceitas sintaticamente; não há
avaliação nem verificação semântica dessas expressões.

## Códigos de saída

| Código | Significado |
| --- | --- |
| 0 | Entrada aceita |
| 1 | Erro sintático |
| 2 | Falta de memória no analisador Bison |
| 10 | Símbolo não reconhecido pelo analisador léxico |
| 20 | Argumentos incorretos ou falha ao abrir, ler ou fechar o arquivo |

O diagnóstico sintático informa o símbolo encontrado e os tokens esperados.
A análise termina no primeiro erro. A contagem de linhas começa em 1.

## Correções e validação

- Makefile com tabulações reais, dependências do cabeçalho gerado, geração
  do Bison antes da compilação, suporte a execução e limpeza repetida.
- Correção do delimitador final do Flex e inclusão dos tokens de parênteses.
- Suporte a arquivos com CRLF e scanner sem dependência de `yywrap`.
- Verificação da abertura/leitura/fechamento e propagação do resultado
  de `yyparse` para o código de saída.
- Diagnósticos com os nomes reais dos tokens, fim de arquivo e lista completa
  de tokens esperados; uso de LAC para melhorar essa lista.
- Preservada a gramática original, incluindo `senao` obrigatório e o mesmo
  nível de precedência para `E` e `OU`.

Validado em Windows com MSYS2: GNU Make 4.4.1, Bison 3.8.2, Flex 2.6.4 e
GCC 16.1.0. A compilação com `-std=c11 -Wall -Wextra -Wpedantic -O2`
passou sem avisos e sem conflitos na gramática.

Foram verificados: exemplo original, expressões com parênteses e operadores
lógicos, seleção e repetição, bloco vazio, entrada CRLF, erros léxico e
sintático, fim de arquivo inesperado, arquivo inexistente e ausência de
argumentos. Também foram verificados compilação paralela, reconstrução
do cabeçalho gerado, compilação incremental e limpeza repetida.

A execução e a compilação em Linux não foram testadas neste ambiente.
