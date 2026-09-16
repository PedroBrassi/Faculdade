# Analisador Léxico — Linguagem "Simples"

Este projeto implementa, com **Flex**, o analisador léxico da linguagem
"Simples", reconhecendo palavras reservadas, tipos, identificadores,
números e operadores.

## Arquivos

- `lexico.l` — especificação léxica (regras Flex).
- `lexico.c` — código C gerado a partir de `lexico.l` (via `flex`).
- `lexico` — executável já compilado.
- `teste.simples` — programa de exemplo na linguagem "Simples".
- `result.txt` — saída obtida ao rodar o analisador sobre `teste.simples`.
- `Makefile` — automatiza a geração, compilação e execução (ver abaixo).

## Pré-requisitos

No Linux/WSL/macOS, instale o Flex, o Make e um compilador C:

```bash
sudo apt-get update
sudo apt-get install flex make gcc
```

## Uso rápido com `make`

O jeito mais simples de compilar e rodar é usar o `Makefile` incluído:

```bash
make        # gera lexico.c a partir de lexico.l e compila o executavel lexico
make run    # compila (se preciso) e roda: ./lexico < teste.simples > result.txt
make clean  # remove lexico.c, lexico e result.txt (mantém lexico.l e teste.simples)
```

`make` e `make build` fazem a mesma coisa (`build` é o alvo padrão). O
`make run` só recompila o que for necessário — se você não alterar
`lexico.l`, ele reaproveita o executável já gerado.

Para usar outro arquivo de teste sem editar o `Makefile`:

```bash
make run TESTE=outro_arquivo.simples SAIDA=outra_saida.txt
```

## Como fazer isso manualmente (sem o `make`)

Caso prefira rodar os comandos um a um:

### Gerar o `.c` a partir do `.l`

Sempre que `lexico.l` for alterado, regenere o `.c`:

```bash
flex -o lexico.c lexico.l
```

### Compilar

```bash
gcc lexico.c -o lexico -lfl
```

> `-lfl` liga a biblioteca do Flex (usada por `yywrap`/funções auxiliares).
> Em algumas distros o nome da lib é `-lfl` mesmo com Flex 2.6+.

### Executar

O analisador lê da entrada padrão (`stdin`), então redirecione o arquivo de teste:

```bash
./lexico < teste.simples > result.txt
```

Ou, para ver a saída direto no terminal:

```bash
./lexico < teste.simples
```