# Implementação de Algoritmos de Caminho Mínimo
### AEDS III — Comparação entre Dijkstra, Bellman-Ford e Duan

Este projeto implementa e compara diferentes algoritmos para o problema do **Single Source Shortest Path (SSSP)**, isto é, o cálculo do menor caminho a partir de um vértice de origem para todos os demais vértices de um grafo ponderado.

Os algoritmos implementados são:

- **Dijkstra**
- **Bellman-Ford**
- **Duan (2025)** — versão inspirada na proposta determinística baseada em redução de fronteira e relaxamento local.

Além do cálculo dos caminhos mínimos, o projeto realiza **medições de tempo de execução**, permitindo uma comparação experimental entre os algoritmos.

---

# Estrutura do Projeto

```txt
.
├── base.c               # Programa principal
├── algoritmos.c         # Implementação algoritmos
├── algoritmos.h         # Protótipos das funções
├── Makefile             # Automação da compilação
├── run_all.sh           # Script para execução
├── data/                # Instâncias de teste (.txt)
├── results.txt          # Resultado dos testes
└── README.md
```

---

# Algoritmos Implementados

## 1. Dijkstra

O algoritmo de Dijkstra é utilizado para calcular caminhos mínimos em grafos com pesos não negativos.

Nesta implementação:

- o grafo é representado por **matriz de adjacência**
- utiliza seleção linear do menor vértice (**sem heap**)
- possui complexidade aproximada:

```math
O(n^2)
```

---

## 2. Bellman-Ford

O algoritmo Bellman-Ford realiza relaxamentos sucessivos de todas as arestas do grafo.

### Características

- suporta pesos negativos (sem ciclos negativos)
- utiliza múltiplas iterações globais
- possui parada antecipada quando não há atualizações

Complexidade:

```math
O(n^3)
```

considerando a representação por **matriz de adjacência**.

---

## 3. Duan (2025)

Foi implementada uma versão inspirada no algoritmo determinístico proposto por **Duan et al. (2025)**.

A implementação utiliza:

- relaxamento local controlado (**tipo Bellman-Ford**)
- redução da fronteira de exploração
- seleção de pivôs
- refinamento local semelhante ao **BMSSP**
- finalização baseada em ordenação local de vértices

Os parâmetros fundamentais são:

```math
k = \lfloor \log^{1/3}(n) \rfloor
```

```math
t = \lfloor \log^{2/3}(n) \rfloor
```

Essa abordagem busca reduzir a dependência de ordenação global presente no algoritmo de Dijkstra.

---

# Formato do Arquivo de Entrada

Os grafos devem estar em arquivos `.txt`.

Formato esperado:

```txt
N M
u1 v1 peso1
u2 v2 peso2
...
uM vM pesoM
```

Onde:

- `N` → número de vértices
- `M` → número de arestas
- `u v peso` → aresta entre os vértices `u` e `v`

## Exemplo

```txt
5 6
0 1 2
0 2 4
1 2 1
1 3 7
2 4 3
3 4 1
```

### Observações

- os vértices devem ser indexados a partir de **0**
- o grafo é considerado **não direcionado**
- pesos devem ser positivos

---

# Compilação do Projeto

O projeto utiliza um **Makefile** para automatizar a compilação.

Para compilar:

```bash
make
```

Esse comando:

1. compila os arquivos `.c`
2. gera os arquivos objeto `.o`
3. cria o executável:

```txt
base
```

A compilação utiliza:

```bash
gcc -Wall -Wextra -O3 -lm
```

### Flags utilizadas

| Flag | Função |
|------|---------|
| `-Wall` | Habilita warnings importantes |
| `-Wextra` | Warnings extras |
| `-O3` | Otimização agressiva |
| `-lm` | Biblioteca matemática (`log`, `pow`, etc.) |

---

# Execução Manual

Após compilar, execute:

```bash
./base arquivo.txt
```

Exemplo:

```bash
./base instancia_exemplo.dat
```

Ou utilizando o Makefile:

```bash
make run FILE=instancia_exemplo.dat
```

## Funcionamento do `make run`

O alvo:

```make
run:
	./$(TARGET) $(FILE)
```

permite executar o programa informando um arquivo dinamicamente.

Exemplo:

```bash
make run FILE=data/grafo100.txt
```

---

# Testes Automáticos

O projeto possui um sistema de benchmark automático utilizando o script:

```txt
run_all.sh
```

Para executar todos os testes:

```bash
make test
```

## Funcionamento do `make test`

O alvo:

```make
test:
	./run_all.sh
```

executa automaticamente o script responsável por:

1. percorrer todos os arquivos `.txt` dentro da pasta:

```txt
data/
```

2. executar os algoritmos para cada instância

3. salvar os resultados no arquivo:

```txt
results.txt
```

Cada teste executa:

- Dijkstra
- Duan
- Bellman-Ford

medindo:

- custo total dos caminhos mínimos
- tempo de execução

---

# Conjunto de Instâncias (`data/`)

Os arquivos presentes na pasta `data/` representam diferentes instâncias de grafos utilizadas para os testes experimentais dos algoritmos.

Os nomes dos arquivos seguem um padrão de nomenclatura que descreve:

- o **modelo do grafo**
- a **quantidade de vértices**
- parâmetros de geração
- identificador da instância

Exemplo:

```txt
data/barabasi-10000-m3-1.txt
```

## Interpretação do nome do arquivo

O nome do arquivo pode ser dividido da seguinte forma:

| Parte | Significado |
|--------|-------------|
| `barabasi` | Modelo do grafo |
| `10000` | Número de vértices |
| `m3` | Parâmetro do modelo (ex.: número de conexões por novo nó) |
| `1` | Identificador da instância |

### Modelos de grafos utilizados

Os experimentos utilizam três tipos de grafos:

| Nome no arquivo | Modelo |
|-----------------|---------|
| `barabasi` | Barabási-Albert |
| `erdos` | Erdős-Rényi |
| `watts` | Watts-Strogatz |

Assim, exemplos válidos incluem:

```txt
barabasi-1000-m3-2.txt
erdos-5000-p10-1.txt
watts-10000-k4-2.txt
```

## Quantidade de instâncias

O conjunto de testes contém:

- **10 instâncias do modelo Barabási-Albert**
- **10 instâncias do modelo Erdős-Rényi**
- **10 instâncias do modelo Watts-Strogatz**

Para cada modelo, existem:

- **2 grafos com 500 vértices**
- **2 grafos com 1.000 vértices**
- **2 grafos com 5.000 vértices**
- **2 grafos com 10.000 vértices**
- **2 grafos com 20.000 vértices**

Essas diferentes escalas permitem analisar o comportamento dos algoritmos em grafos de tamanhos variados, possibilitando uma comparação experimental de desempenho e escalabilidade.

# Formato da Saída

A saída do programa segue o formato:

```txt
Dijkstra: <custo> <tempo>
Duan: <custo> <tempo>
bellmanFord: <custo> <tempo>
```

Exemplo:

```txt
Dijkstra: 658195.000000 0.175240
Duan: 658195.000000 0.102341
bellmanFord: 658195.000000 1.482311
```

Onde:

| Valor | Significado |
|--------|-------------|
| Primeiro | Soma das menores distâncias |
| Segundo | Tempo de execução (segundos) |

---

# Arquivo de Resultados

Após executar:

```bash
make test
```

os resultados são armazenados automaticamente em:

```txt
results.txt
```

Formato esperado:

```txt
===== Teste 1 =====
Arquivo: data/grafo100.txt
Dijkstra: ...
Duan: ...
bellmanFord: ...

===== Teste 2 =====
Arquivo: data/grafo500.txt
...
```

---

# Limpeza do Projeto

Para remover arquivos compilados:

```bash
make clean
```

Esse comando remove:

- executável
- arquivos `.o`
- `results.txt`

---

# Comparação Teórica dos Algoritmos

| Algoritmo | Complexidade |
|------------|--------------|
| Dijkstra | `O(n²)` |
| Bellman-Ford | `O(n³)` |
| Duan (simplificado) | `~O(n² log^(1/3)n)` |

### Interpretação

- **Dijkstra** → tende a ser mais rápido em grafos pequenos e médios
- **Bellman-Ford** → mais lento devido às múltiplas relaxações globais
- **Duan** → busca reduzir relaxamentos desnecessários através de exploração local controlada

# Autor

    Disciplina: AEDS III
    Professor: Iago A. Carvalho
    Curso: Ciência da Computação – UNIFAL
    Aluno: Pedro Brassi Luccas
    Data: 14/05/2026
    
Projeto desenvolvido para a disciplina de **AEDS III**, com foco na análise experimental de algoritmos de caminhos mínimos em grafos.
