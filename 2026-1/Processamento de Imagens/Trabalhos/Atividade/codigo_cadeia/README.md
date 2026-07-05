# Código de Cadeia (Freeman Chain Code) - C

Implementação em C da atividade prática "Código de Cadeia": um par de
programas que **codifica** o contorno de um objeto em uma imagem PBM
para um arquivo compacto `.ctn`, e **decodifica** um arquivo `.ctn` de
volta para uma imagem PBM reconstruída.

    Disciplina: Processamento de Imagens
    Professor: Luiz Eduardo da Silva
    Curso: Ciência da Computação – UNIFAL
    Aluno: Pedro Brassi Luccas
    Data: 05/07/2026

## Estrutura do projeto

```
codigo_cadeia/
├── Makefile
├── README.md
├── include/
│   ├── pbm.h         # leitura/escrita de imagens PBM
│   ├── chaincode.h   # rastreamento de contorno e conversão hexadecimal
│   └── ctn.h         # leitura/escrita do formato .ctn
├── src/
│   ├── pbm.c
│   ├── chaincode.c
│   ├── ctn.c
│   ├── encode.c      # main do programa `codificador`
│   └── decode.c      # main do programa `decodificador`
├── examples/
│   ├── exemplo.ctn         # exemplo do enunciado (7x7, início (2,1), 13 direções)
│   ├── exemplo_forma.pbm   # anel quadrado 5x5 (contorno fechado) para testar a codificação
│   └── estrela.pbm         # exemplo teste - estrela de 5 pontas 41x41
├── bin/        # criada pelo make - contém os executáveis (codificador, decodificador)
├── build/      # criada pelo make - contém os arquivos .o intermediários da compilação
└── results/    # criada pelo make - contém os arquivos .ctn/.pbm gerados ao rodar os programas
```

`bin/`, `build/` e `results/` não fazem parte do repositório: são criadas
automaticamente ao rodar `make` e removidas por completo ao rodar
`make clean`.

## Compilação

Basta rodar, na raiz do projeto:

```bash
make
```

Isso gera dois executáveis em `bin/`:

- `bin/codificador`
- `bin/decodificador`

E também cria a pasta `results/`, vazia, destinada a receber os
arquivos `.ctn`/`.pbm` gerados ao rodar os programas (veja `make test`
logo abaixo, ou rode os programas manualmente apontando a saída para
essa pasta, ex.: `./bin/codificador examples/exemplo_forma.pbm results/saida.ctn`).

Para limpar tudo (`build/`, `bin/` e `results/`):

```bash
make clean
```

Para compilar e já rodar um teste rápido com os exemplos (os arquivos
gerados vão parar em `results/`):

```bash
make test
```

Para rodar o codificador **e** o decodificador em qualquer imagem `.pbm`
que esteja na pasta `examples/`, sem precisar digitar os comandos
completos toda vez, basta chamar `make` seguido do nome do arquivo:

```bash
make <nome_da_imagem>.pbm
```

Exemplo, usando `examples/estrela.pbm`:

```bash
make estrela.pbm
```

Isso automaticamente:

1. compila o projeto, se ainda não tiver sido compilado (mesma coisa
   que o `make` sozinho faria);
2. roda `codificador` em `examples/estrela.pbm`, gerando
   `results/estrela.ctn`;
3. roda `decodificador` em `results/estrela.ctn`, gerando
   `results/estrela_reconstruido.pbm` 

De forma geral, para uma imagem `examples/<nome>.pbm`, o comando
`make <nome>.pbm` gera `results/<nome>.ctn` e
`results/<nome>_reconstruido.pbm`. Se o arquivo pedido não existir em
`examples/`, o `make` avisa com `No rule to make target...` em vez de
rodar os programas.

## Uso

### Codificador (imagem → `.ctn`)

```bash
./bin/codificador <imagem_entrada.pbm> <arquivo_saida.ctn>
```

Exemplo:

```bash
./bin/codificador examples/exemplo_forma.pbm saida.ctn
```

O programa imprime no terminal as dimensões, o ponto inicial, o número
de direções e a cadeia hexadecimal gerada, além de gravar o arquivo
`.ctn`.

### Decodificador (`.ctn` → imagem)

```bash
./bin/decodificador <arquivo_entrada.ctn> <imagem_saida.pbm>
```

Exemplo:

```bash
./bin/decodificador examples/exemplo.ctn saida.pbm
```

A imagem PBM gerada é sempre no formato **P1 (ASCII)**.

> O leitor de PBM (`ler_pbm`) aceita tanto **P1 (ASCII)** quanto **P4
> (binário)** como entrada. A escrita é sempre em P1.

## Formato do arquivo `.ctn`

```
<nl> <nc>
<linha_inicial> <coluna_inicial>
<n_direcoes>
<cadeia_hexadecimal>
```

Exemplo (`examples/exemplo.ctn`):

```
7 7
2 1
13
F700124648
```

## Sistema de direções (8-conectado)

Convenção usada:

```
3 2 1
4 x 0
5 6 7
```

| Código | Deslocamento (Δlinha, Δcoluna) | Direção   |
|:------:|:-------------------------------:|-----------|
| 0      | (0, +1)                          | Leste     |
| 1      | (-1, +1)                         | Nordeste  |
| 2      | (-1, 0)                          | Norte     |
| 3      | (-1, -1)                         | Noroeste  |
| 4      | (0, -1)                          | Oeste     |
| 5      | (+1, -1)                         | Sudoeste  |
| 6      | (+1, 0)                          | Sul       |
| 7      | (+1, +1)                         | Sudeste   |

## Como funciona a codificação

1. **Localização do ponto inicial**: varre a imagem de cima para baixo
   e da esquerda para a direita até achar o primeiro pixel com valor 1.
2. **Rastreamento do contorno**: usa o algoritmo de **rastreamento por
   vizinhança de Moore** (Moore-Neighbor Tracing), 8-conectado. A cada
   passo, a busca pelo próximo pixel de contorno começa a partir da
   direção seguinte à direção oposta à de chegada, percorrendo os 8
   vizinhos em ordem crescente (sentido anti-horário, conforme a
   numeração do enunciado) até achar o próximo pixel de contorno. O
   rastreamento termina quando o algoritmo retorna ao ponto inicial
   (contorno fechado) ou quando não há mais vizinhos de contorno
   (objeto de 1 pixel).
3. **Geração da cadeia**: cada passo do rastreamento gera um código de
   direção (0 a 7).
4. **Conversão para binário**: cada direção vira 3 bits (000 a 111).
5. **Compactação em hexadecimal**: os bits de todas as direções são
   concatenados e agrupados em blocos de 4. Caso o total de bits não
   seja múltiplo de 4, o final é completado com zeros (padding) até o
   próximo múltiplo de 4,  exatamente como no exemplo do enunciado (13
   direções × 3 bits = 39 bits → 1 bit de padding → 40 bits → 10 dígitos
   hexadecimais → `F700124648`).

## Como funciona a decodificação

1. Lê `nl`, `nc`, o ponto inicial, o número de direções e a cadeia
   hexadecimal.
2. Converte cada dígito hexadecimal em 4 bits.
3. Agrupa os bits em blocos de 3, obtendo de volta os códigos de
   direção (os bits de padding do final são descartados).
4. A partir do ponto inicial, aplica cada deslocamento sequencialmente,
   marcando cada pixel visitado como 1 numa imagem `nl x nc`
   inicialmente zerada.
5. Grava o resultado como uma imagem PBM (P1).

## Validação feita

- O arquivo `examples/exemplo.ctn` é exatamente o exemplo do
  enunciado (`7 7 / 2 1 / 13 / F700124648`). Decodificá-lo reproduz
  pixel a pixel a sequência de pontos calculada manualmente no PDF:
  `(2,1) → (3,2) → (4,1) → (5,1) → (5,2) → (5,3) → (5,4) → (4,4) →
  (3,4) → (2,4) → (1,5) → (1,4) → (1,3) → (1,2)`.
- `examples/exemplo_forma.pbm` é um anel quadrado 5×5 (contorno
  fechado). Codificá-lo e depois decodificar o `.ctn` gerado produz
  uma imagem **idêntica, pixel a pixel**, à original, validando o
  ciclo completo codificação → decodificação.

