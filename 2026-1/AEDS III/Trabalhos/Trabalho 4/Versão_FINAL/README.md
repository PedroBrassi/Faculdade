# Problema da Partição (*Partition Problem*) – Análise Comparativa de Heurísticas

Este repositório contém a implementação, os geradores de instâncias, os algoritmos heurísticos e os relatórios estatísticos desenvolvidos para a resolução e análise computacional do **Problema da Partição (*Partition Problem*)**, realizado como requisito avaliativo da disciplina **Algoritmos e Estruturas de Dados III (AEDS III)** do Departamento de Ciência da Computação da Universidade Federal de Alfenas (**UNIFAL-MG**).

O objetivo do projeto consiste em comparar diferentes abordagens heurísticas e meta-heurísticas aplicadas a um problema clássico de otimização combinatória pertencente à classe **NP-completa**, avaliando o desempenho de cada método em termos de **qualidade da solução** e **tempo computacional**.

---

## 1. O Problema da Partição

O **Problema da Partição (*Partition Problem*)** consiste em dividir um conjunto de números inteiros positivos em dois subconjuntos disjuntos de forma que a diferença absoluta entre as somas dos seus elementos seja mínima.

Formalmente, dado um conjunto:

$$
S = {x_1, x_2, x_3, ..., x_n}
$$

o objetivo é encontrar dois subconjuntos:

$$
A \cup B = S
$$

$$
A \cap B = \emptyset
$$

de modo a minimizar a função objetivo (*fitness*):

$$
\text{Fitness} =
\left|
\sum_{i \in A} x_i
-
\sum_{j \in B} x_j
\right|
$$

Quanto menor o valor do *fitness*, melhor é o balanceamento entre os subconjuntos. O valor ideal é **0**, indicando uma partição perfeitamente equilibrada.

---

## 2. Algoritmos Implementados

O sistema implementa e compara quatro abordagens distintas para resolução do problema.

### 2.1 Partition Puro (*Baseline*)

Implementação determinística utilizada como **linha de base comparativa** (*baseline*). Todos os elementos da instância são alocados no grupo **A**, mantendo o grupo **B** vazio.

Embora não produza boas soluções, essa abordagem representa um cenário de referência para medir os ganhos obtidos pelas heurísticas.

**Complexidade:** `O(N)`

---

### 2.2 Algoritmo Guloso (LPT Adaptado)

Heurística baseada na estratégia **Longest Processing Time (LPT)**.

Inicialmente, os elementos são ordenados em ordem decrescente utilizando **QuickSort**. Em seguida, cada elemento é inserido no subconjunto que possui a menor soma acumulada no instante da decisão.

Essa abordagem busca equilibrar as cargas de maneira incremental, apresentando excelente desempenho computacional.

**Etapas do algoritmo:**

1. Ordenação decrescente dos elementos;
2. Comparação das somas parciais dos grupos;
3. Inserção gulosa no grupo de menor carga.

**Complexidade:** `O(N log N)`

---

### 2.3 Simulated Annealing (Busca Local)

Meta-heurística inspirada no processo físico de **recozimento térmico (*annealing*)** utilizado na metalurgia.

A solução é representada por um vetor binário indicando a qual subconjunto cada elemento pertence. Em cada iteração, o algoritmo explora soluções vizinhas através de perturbações estocásticas (*bit flip*), podendo aceitar temporariamente soluções piores para escapar de mínimos locais.

O método implementado inclui uma otimização importante: o cálculo do *fitness* vizinho é realizado de forma **incremental em tempo constante `O(1)`**, evitando o recálculo completo das somas.

### Características da implementação

* Geração de vizinhos via **Bit Flip**;
* Índices embaralhados utilizando **Fisher–Yates Shuffle**;
* Aceitação probabilística de soluções piores;
* Resfriamento gradual da temperatura;
* Atualização incremental do *fitness*.

---

### 2.4 Algoritmo Genético

Meta-heurística baseada nos princípios da **seleção natural e evolução biológica**.

Cada solução é representada como um **cromossomo binário**, onde cada gene indica o subconjunto associado ao elemento correspondente.

O algoritmo opera sobre uma população de indivíduos, utilizando operadores genéticos para explorar o espaço de busca.

### Operadores utilizados

#### Seleção

* **Torneio (`k = 3`)**

#### Cruzamento (*Crossover*)

* **Single-Point Crossover**

#### Mutação

* Mutação por inversão de bit (*bit flip*)

#### Estratégias adicionais

* **Elitismo puro**
* **Mutação adaptativa**, aumentando a taxa nas gerações finais para reduzir convergência prematura.

---

## 3. Estrutura do Projeto

A arquitetura do projeto está organizada de forma modular:

```text
├── include/                  # Arquivos de cabeçalho (.h)
│   ├── leitor.h
│   ├── partition.h
│   ├── simulatedAnnealing.h
│   ├── genetico.h
│   └── guloso.h
│
├── src/                      # Código-fonte das implementações (.c)
│   ├── leitor.c
│   ├── partition.c
│   ├── simulatedAnnealing.c
│   ├── genetico.c
│   ├── guloso.c
│   └── main.c
│
├── tools/                    # Ferramentas auxiliares
│   └── instancias.c          # Gerador de instâncias
│
├── resultados/               # Relatórios estatísticos gerados
│   ├── resultadosTabela.txt
│   └── resultadoMedia.txt
│
├── Makefile
└── README.md
```

---

## 4. Configuração Experimental

Para garantir diversidade estatística e avaliar o comportamento assintótico dos algoritmos, foi desenvolvido um gerador automático de instâncias (`tools/instancias.c`).

Foram criadas **35 instâncias de teste**, distribuídas em **7 tamanhos distintos**:

$$
N \in {100, 200, 500, 1000, 2500, 5000, 10000}
$$

Cada tamanho possui **5 variações independentes**, produzidas sob diferentes limites máximos de valores (`max_val`):

```text
100
1000
5000
25000
100000
```

Essa variação altera significativamente:

* dispersão dos números;
* densidade de soluções próximas do ótimo;
* dificuldade do espaço de busca.

### Controle estatístico

Cada algoritmo foi executado:

**5 vezes por instância (*runs independentes*)**

Totalizando:

```text
35 instâncias × 5 execuções = 175 execuções por algoritmo
```

A utilização de múltiplas execuções reduz o impacto da aleatoriedade inerente às meta-heurísticas.

---

## 5. Compilação e Execução

### Pré-requisitos

Certifique-se de possuir:

* compilador `gcc` ou `cc`;
* `make`;
* sistema Linux, macOS ou MinGW/WSL no Windows.

---

### Limpar compilações anteriores (opcional)

```bash
make clean
```

### Compilar o projeto

```bash
make
```

### Executar os experimentos

```bash
make run
```

Durante a execução, o terminal exibirá o progresso da bateria de testes em tempo real:

```text
========================================================================
   Iniciando Bateria Geral de Experimentos
========================================================================

[PartitionPuro]      - Finalizado para 100   - Tempo total: 0.000854 s
[SimulatedAnnealing] - Finalizado para 100   - Tempo total: 0.054120 s
[AlgoritmoGenetico]  - Finalizado para 100   - Tempo total: 0.124512 s
[Guloso]             - Finalizado para 100   - Tempo total: 0.003011 s
----------------------------------------------------------------------------
```

Os relatórios finais serão gerados automaticamente no diretório:

```text
resultados/
```

Arquivos produzidos:

* `resultadosTabela.txt` → resultados detalhados;
* `resultadoMedia.txt` → médias consolidadas.

---

## 6. Resultados Experimentais

Os experimentos totalizaram aproximadamente:

```text
387.99 segundos
```

de tempo computacional global.

Os resultados demonstraram comportamentos distintos entre os algoritmos conforme o aumento da dimensão do problema.

### Qualidade das soluções

* O **Algoritmo Genético** apresentou melhor desempenho em instâncias pequenas e médias (`N ≤ 500`), explorando eficientemente o espaço de busca através da diversidade populacional.

* O **Simulated Annealing** apresentou maior robustez e estabilidade em instâncias grandes (`1000 ≤ N ≤ 5000`), alcançando *fitness* próximos do ótimo com baixo custo computacional.

* O **Algoritmo Guloso (LPT)** obteve resultados surpreendentemente competitivos em grandes instâncias, alcançando o melhor *fitness* médio em `N = 10000`.

### Tempo computacional

* O **Partition Puro** apresentou o menor custo computacional, porém com baixa qualidade de solução.

* O **Simulated Annealing** manteve excelente escalabilidade, variando aproximadamente entre `0.003 s` e `0.064 s`.

* O **Algoritmo Genético** apresentou crescimento significativo no tempo de execução conforme `N` aumentou, alcançando aproximadamente `11.18 s` em `N = 10000`.

* O **Algoritmo Guloso** apresentou execução praticamente instantânea em todas as escalas testadas.

---

## 7. Principais Conclusões

### Algoritmo Genético — Melhor desempenho em pequenas instâncias

Em problemas de menor dimensão (`N ≤ 500`), o Algoritmo Genético obteve soluções de elevada qualidade devido à diversidade populacional e aos operadores evolutivos de cruzamento e mutação.

Entretanto, o custo computacional cresceu significativamente em instâncias maiores.

---

### Simulated Annealing — Melhor equilíbrio geral

O Simulated Annealing apresentou o melhor equilíbrio entre:

* qualidade da solução;
* escalabilidade;
* custo computacional.

A atualização incremental do *fitness* em `O(1)` permitiu elevado número de iterações mantendo baixo tempo de execução.

---

### Algoritmo Guloso — Excelente desempenho em larga escala

Para `N = 10000`, o algoritmo guloso obteve o melhor *fitness* médio com tempo praticamente instantâneo.

Esse comportamento sugere que, em grandes conjuntos, a densidade de soluções próximas do ótimo favorece abordagens simples baseadas em balanceamento incremental.

---

## 8. Tecnologias Utilizadas

* **Linguagem:** C
* **Compilador:** GCC
* **Build System:** Makefile
* **Paradigmas:** Heurísticas, Meta-heurísticas e Computação Evolutiva

---

## 9. Autores

Projeto desenvolvido por:

* Isabela M. Andrade
* Júlia A. da Rocha
* Otávio de Oliveira
* Pedro B. Luccas

**Universidade Federal de Alfenas (UNIFAL-MG)**
**Departamento de Ciência da Computação – AEDS III**
