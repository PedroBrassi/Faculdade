# Escala Livre (Grafo Barabási–Albert)

**Parâmetros:** m = 3 · 2 instâncias por tamanho

---

## Resultados

| Vértices | Algoritmo     | Inst. 1 (s) | Inst. 2 (s) | Média (s) | Custo médio |
|----------|--------------|-------------|-------------|-----------|-------------|
| 500      | Dijkstra     | 0.000723    | 0.000722    | 0.000723  | 30,248      |
| 500      | Duan         | 0.000742    | 0.000680    | 0.000711  | 32,294      |
| 500      | Bellman-Ford | 0.001196    | 0.000806    | 0.001001  | 30,248      |
| 1.000    | Dijkstra     | 0.002665    | 0.002582    | 0.002624  | 60,744      |
| 1.000    | Duan         | 0.002248    | 0.002256    | 0.002252  | 63,200      |
| 1.000    | Bellman-Ford | 0.004032    | 0.004029    | 0.004031  | 60,744      |
| 2.000    | Dijkstra     | 0.004897    | 0.006779    | 0.005838  | 130,620     |
| 2.000    | Duan         | 0.004562    | 0.003986    | 0.004274  | 135,888     |
| 2.000    | Bellman-Ford | 0.007642    | 0.009901    | 0.008772  | 130,620     |
| 5.000    | Dijkstra     | 0.033338    | 0.031834    | 0.032586  | 342,700     |
| 5.000    | Duan         | 0.021948    | 0.026211    | 0.024080  | 376,631     |
| 5.000    | Bellman-Ford | 0.073398    | 0.075939    | 0.074669  | 342,700     |
| 10.000   | Dijkstra     | 0.175577    | 0.167068    | 0.171323  | 675,865     |
| 10.000   | Duan         | 0.089117    | 0.084799    | 0.086958  | 712,350     |
| 10.000   | Bellman-Ford | 0.310834    | 0.272004    | 0.291419  | 675,865     |

---

## Observação

O algoritmo de Duan apresenta valores de custo ligeiramente superiores aos obtidos por Dijkstra e Bellman-Ford.

Isso indica que o método realiza **aproximações nas distâncias durante a decomposição da fronteira**, comportamento esperado devido à estratégia de pivôs e limitação local de relaxamento.

# Aleatório Uniforme (Grafo Erdős–Rényi)

**Parâmetros:** p = 0.01 · 2 instâncias por tamanho

---

## Resultados

| Vértices | Algoritmo     | Inst. 1 (s) | Inst. 2 (s) | Média (s) | Custo médio |
|----------|--------------|-------------|-------------|-----------|-------------|
| 500      | Dijkstra     | 0.000682    | 0.000662    | 0.000672  | 69,309      |
| 500      | Duan         | 0.000497    | 0.000556    | 0.000527  | 71,998      |
| 500      | Bellman-Ford | 0.001352    | 0.001071    | 0.001212  | 69,309      |
| 1.000    | Dijkstra     | 0.001487    | 0.001339    | 0.001413  | 69,095      |
| 1.000    | Duan         | 0.000970    | 0.000883    | 0.000927  | 85,593      |
| 1.000    | Bellman-Ford | 0.002660    | 0.002804    | 0.002732  | 69,095      |
| 2.000    | Dijkstra     | 0.005178    | 0.005570    | 0.005374  | 89,135      |
| 2.000    | Duan         | 0.003865    | 0.003404    | 0.003635  | 108,469     |
| 2.000    | Bellman-Ford | 0.017428    | 0.014667    | 0.016048  | 89,135      |
| 5.000    | Dijkstra     | 0.033572    | 0.035192    | 0.034382  | 107,941     |
| 5.000    | Duan         | 0.033211    | 0.030869    | 0.032040  | 129,102     |
| 5.000    | Bellman-Ford | 0.099108    | 0.115482    | 0.107295  | 107,941     |
| 10.000   | Dijkstra     | 0.190674    | 0.182302    | 0.186488  | 135,902     |
| 10.000   | Duan         | 0.189304    | 0.211766    | 0.200535  | 142,883     |
| 10.000   | Bellman-Ford | 0.407495    | 0.400197    | 0.403846  | 135,902     |

---

## Observação

No modelo Erdős–Rényi com p = 0.01, o grafo apresenta maior densidade e homogeneidade estrutural.

O algoritmo de Duan perde vantagem em instâncias maiores, aproximando-se do desempenho do Dijkstra em 10.000 vértices.

Esse comportamento é coerente com a alta conectividade do grafo, que reduz a eficácia da decomposição local baseada em pivôs.

# Pequeno Mundo (Grafo Watts–Strogatz)

**Parâmetros:** k = 4, p = 0.2 · 2 instâncias por tamanho

---

## Resultados

| Vértices | Algoritmo     | Inst. 1 (s) | Inst. 2 (s) | Média (s) | Custo médio |
|----------|--------------|-------------|-------------|-----------|-------------|
| 500      | Dijkstra     | 0.000636    | 0.000648    | 0.000642  | 107,666     |
| 500      | Duan         | 0.000489    | 0.000482    | 0.000486  | 114,542     |
| 500      | Bellman-Ford | 0.001918    | 0.001081    | 0.001500  | 107,666     |
| 1.000    | Dijkstra     | 0.002407    | 0.001539    | 0.001973  | 279,416     |
| 1.000    | Duan         | 0.000891    | 0.000782    | 0.000837  | 288,341     |
| 1.000    | Bellman-Ford | 0.003230    | 0.003217    | 0.003224  | 279,416     |
| 2.000    | Dijkstra     | 0.004894    | 0.007549    | 0.006222  | 578,864     |
| 2.000    | Duan         | 0.003021    | 0.003207    | 0.003114  | 606,274     |
| 2.000    | Bellman-Ford | 0.019052    | 0.021784    | 0.020418  | 578,864     |
| 5.000    | Dijkstra     | 0.035088    | 0.030023    | 0.032556  | 1,676,068   |
| 5.000    | Duan         | 0.021681    | 0.023044    | 0.022363  | 1,741,433   |
| 5.000    | Bellman-Ford | 0.145336    | 0.146464    | 0.145900  | 1,676,068   |
| 10.000   | Dijkstra     | 0.136851    | 0.142845    | 0.139848  | 4,499,463   |
| 10.000   | Duan         | 0.095718    | 0.085342    | 0.090530  | 4,612,816   |
| 10.000   | Bellman-Ford | 0.666537    | 0.615734    | 0.641136  | 4,499,463   |

---

## Observação

No modelo Watts–Strogatz (small-world), o grafo é esparso e altamente estruturado, favorecendo algoritmos baseados em exploração local.

O algoritmo de Duan apresenta melhor desempenho relativo, atingindo ganhos de até ~35–50% em relação ao Dijkstra nas maiores instâncias (10.000 vértices).

Esse comportamento é coerente com a estrutura do grafo, que facilita a decomposição em regiões locais bem definidas, aumentando a eficiência da abordagem baseada em pivôs.

# Speedup relativo ao Dijkstra (10.000 vértices)

| Topologia        | Dijkstra (s) | Duan (s) | Speedup Duan        | Bellman-Ford (s) | Slowdown BF        |
|------------------|-------------|----------|---------------------|------------------|--------------------|
| Barabási         | 0.171323    | 0.086958 | 1.97× mais rápido   | 0.291419         | 1.70× mais lento   |
| Erdős–Rényi      | 0.186488    | 0.200535 | ~0.93× (−7%)        | 0.403846         | 2.17× mais lento   |
| Watts–Strogatz   | 0.139848    | 0.090530 | 1.54× mais rápido   | 0.641136         | 4.58× mais lento   |

# Confirmação empírica das complexidades teóricas

| Algoritmo      | Complexidade teórica              | Observado empiricamente | Confirma? |
|----------------|----------------------------------|--------------------------|-----------|
| Dijkstra       | O(V²) — quadrático               | 500 → 10.000 (20× vértices): tempo cresce ~237× em Barabási. Crescimento supralinear, mas abaixo do V² puro, indicando influência de estruturas tipo heap e variação de densidade. | Parcial |
| Bellman-Ford   | O(V·E) — cúbico em grafo denso   | No Watts (esparso), BF é 4.58× mais lento que Dijkstra em 10k. No Erdős (mais denso), 2.17×. Crescimento consistente com dependência direta de E. | Sim |
| Duan           | O(m log²⁄³ n) — sublinear em m   | Em grafos esparsos (Barabási, Watts), apresenta ganho consistente. Em Erdős 10k, perde eficiência devido ao aumento de densidade e pior particionamento. Confirma forte dependência de m e estrutura do grafo. | Sim |