# Exemplos de Flex

Esta pasta reúne 5 exemplos progressivos de analisadores léxicos escritos
com **Flex**, cada um ilustrando um recurso diferente da ferramenta.

## Arquivos

| Arquivo | O que faz |
|---|---|
| `01_regra_padrao.l` | Exemplo mínimo: nenhuma regra explícita, só a regra padrão do Flex (copia a entrada para a saída, como `cat`). |
| `02_extrai_numeros.l` | Extrai apenas os números de um texto, descartando o resto. |
| `03_numera_linhas.l` | Numera as linhas de um arquivo (estilo `cat -n`). |
| `04_conta_identificadores.l` | Conta quantos identificadores (palavra iniciada por letra) existem no texto. |
| `05_conta_texto.l` | Conta caracteres, linhas e palavras (estilo `wc`). |
| `teste.txt` | Arquivo de entrada usado para testar os 5 exemplos. |
| `Makefile` | Automatiza a compilação, execução e limpeza (ver abaixo). |

> Os arquivos foram renomeados de `um/dois/tres/quatro/cinco` para nomes
> que descrevem o que cada exemplo faz, o que ajuda a identificar cada
> um sem precisar abrir o conteúdo.

Cada arquivo `.l` tem um comentário no topo explicando as regras, e
comentários indentados dentro da seção de regras destacando pontos que
merecem atenção (detalhados também mais abaixo neste README).

## Pré-requisitos

```bash
sudo apt-get update
sudo apt-get install flex make gcc
```

## Uso rápido com `make`

```bash
make        # compila os 5 exemplos (gera e depois remove os .c intermediarios)
make run    # compila (se preciso) e executa os 5 exemplos com teste.txt
make clean  # remove os binarios e quaisquer .c gerados
```

`make` e `make build` fazem a mesma coisa. O `make` usa regras
genéricas (`%.c: %.l` e `%: %.c`), então os arquivos `.c` gerados pelo
Flex são tratados como intermediários e apagados automaticamente após
a compilação — se quiser inspecionar o `.c` de um exemplo específico,
gere-o isoladamente:

```bash
flex -o 02_extrai_numeros.c 02_extrai_numeros.l
```

Para rodar só um exemplo manualmente após compilar:

```bash
./02_extrai_numeros < teste.txt
```

`03_numera_linhas` é diferente: ele recebe o nome do arquivo como
**argumento de linha de comando**, não via `stdin`:

```bash
./03_numera_linhas teste.txt
```

## Conceitos usados nos exemplos

- **`%{ ... %}`** (bloco de código): tudo dentro é copiado literalmente
  para o `.c` gerado, fora de qualquer regra — é onde variáveis
  globais como `conta`, `count`, `car`, `lin`, `pal` são declaradas.
- **`ECHO`**: macro do Flex equivalente a `printf("%s", yytext)` — imprime
  o texto que casou com a regra atual.
- **`yytext`**: variável do Flex com o texto que casou com a regra.
- **`yyleng`**: comprimento (em caracteres) desse texto.
- **Regra padrão**: qualquer trecho da entrada que não case com
  nenhuma regra definida é copiado para a saída automaticamente
  (é o que `01_regra_padrao.l` demonstra, por não ter nenhuma regra).
- **`yywrap()`**: chamada pelo Flex quando a entrada acaba; retornar
  `1` diz "não há mais arquivos para processar, pode parar".

## Observações que valem a leitura (já testadas neste ambiente)

1. **De onde vem o `main()` em `01_regra_padrao.l`?**
   Esse arquivo não define `main()` nem `yywrap()` em código C — e
   ainda assim compila e roda. Isso acontece porque a biblioteca do
   Flex (`-lfl`) já traz um `main()` padrão (que só chama `yylex()`) e
   um `yywrap()` padrão (que retorna `1`), usados quando o próprio
   arquivo `.l` não os define.

2. **`teste.txt` tem uma particularidade**: a primeira linha termina
   com `\r\n` (estilo Windows) e a segunda linha **não tem quebra de
   linha no final** (comum em arquivos editados sem salvar uma linha
   vazia extra). Sem tratamento especial, o Flex só reconhece uma
   linha quando encontra o caractere `\n` — então a última linha de um
   arquivo assim ficaria sem numeração/contagem. **Isso já foi
   corrigido nos dois exemplos afetados**:
   - `03_numera_linhas.l` ganhou uma segunda regra (`^(.*)`, sem exigir
     `\n`) que cobre a última linha quando ela não termina com quebra
     de linha, imprimindo o número dela normalmente.
   - `05_conta_texto.l` ganhou uma regra `<<EOF>>` que soma 1 a `lin`
     se a última linha não tiver terminado com `\n`.
   Em ambos, o resultado agora fica correto (2 linhas) independente de
   o arquivo terminar ou não com quebra de linha — testei os dois
   casos.
   Uma pegadinha secundária que **continua existindo** em
   `03_numera_linhas.l`: como o arquivo usa `\r\n` na 1ª linha, esse
   `\r` fica incluído dentro do texto capturado por `(.*)` e é
   impresso junto — o que pode causar um efeito visual estranho no
   terminal (o cursor volta para o início da linha antes do `\n`
   real). Isso é só cosmético, não afeta a numeração.

3. **`div`, em outros exercícios da disciplina, é tratado como palavra
   reservada** (operador de divisão) — vale lembrar disso se algum dos
   próximos exemplos combinar identificadores com palavras-chave: a
   ordem das regras no `.l` importa, porque o Flex sempre escolhe a
   regra que gera o casamento mais longo e, em caso de empate, a que
   aparece primeiro no arquivo.

## Estrutura geral de um arquivo `.l`

```lex
definicoes            /* macros de regex, ex.: digit [0-9] */
%{
  codigo C livre      /* variaveis globais, includes, etc. */
%}
%%
regra1  { acao1 }
regra2  { acao2 }
%%
codigo C livre        /* main(), yywrap(), funcoes auxiliares */
```
