# Etapa 2 — Deduplicação e Sensibilidade

## Objetivo

Usar o manifesto gerado na Etapa 1 para: (1) encontrar duplicatas exatas via
SHA-256; (2) medir o quanto imagens perceptualmente parecidas atravessam
`train`/`test` ou classes diferentes, em vários limiares de distância; e
(3) agrupar essas imagens parecidas em componentes conexos, para que a
curadoria visual (Etapa 3) julgue clusters inteiros em vez de pares soltos.

Nenhum dos dois scripts desta pasta modifica, remove ou reclassifica
imagens — tudo aqui é levantamento para decisão humana posterior.

## Conteúdo desta pasta

```
Etapa 2 - Deduplicacao e Sensibilidade/
├── deduplicacao_exata.py        # duplicatas SHA-256 + sensibilidade perceptual (pHash/dHash)
├── componentes_conexos.py       # agrupamento em componentes conexos (grafo) em T=0
└── resultados/
    ├── duplicatas_exatas.csv         # uma linha por arquivo em grupo SHA-256 repetido
    ├── resumo_grupos_exatos.csv      # uma linha por grupo SHA-256
    ├── analise_sensibilidade.csv     # pares candidatos por hash e limiar (T=0,3,5,8,10)
    ├── pares_t0_para_revisao.csv     # pares candidatos em T=0 (split ou classe diferente)
    ├── resumo_componentes_t0.csv     # uma linha por componente conexo em T=0
    ├── imagens_por_componente_t0.csv # mapeamento imagem → componente
    ├── componente_te_gl_68_t0.csv    # caso exploratório: o componente da imagem "hub"
    └── LEIA_OS_RESULTADOS.md         # dicionário de dados dos CSVs acima
```

Os dois scripts leem o **mesmo manifesto** da Etapa 1
(`Etapa 1 - Auditoria da Base/resultados/manifesto_base_dados.csv`) de forma
independente — nenhum dos dois depende da saída do outro. Podem ser
executados em qualquer ordem.

## Como executar

```bash
python deduplicacao_exata.py
python componentes_conexos.py
```

Pré-requisitos: `pandas`, `numpy`, `scipy` e `networkx` instalados, e o
manifesto da Etapa 1 já gerado.

## `deduplicacao_exata.py`

Já documentado em detalhe no `LEIA_OS_RESULTADOS.md`. Resumo: agrupa
arquivos com SHA-256 idêntico (153 grupos, 340 arquivos, nenhum atravessando
`train`/`test` ou classe); depois compara pHash e dHash de todas as imagens
por distância de Hamming (`scipy.spatial.distance.pdist`), tabula a
sensibilidade em T={0,3,5,8,10}, e exporta a fila de pares candidatos em
T=0 que têm split ou classe diferentes.

Validações antes de processar: SHA-256 sem valores ausentes, `relative_path`
preenchido e único, `split_assigned`/`class_label` preenchidos, e hashes
perceptuais com exatamente 64 bits (16 caracteres hex). Qualquer violação
interrompe o script com uma mensagem, em vez de gerar contagens erradas.

## `componentes_conexos.py`

Implementa o agrupamento em componentes conexos que estava pendente no
plano original. Ideia: cada imagem é um nó; duas imagens são ligadas se
tiverem o **mesmo** pHash ou o **mesmo** dHash (distância zero — T=0). Ao
usar `networkx.connected_components`, cadeias transitivas são capturadas
automaticamente (se A~B e B~C, as três caem no mesmo componente, mesmo que
A e C não tenham hash igual entre si) — é exatamente o comportamento que o
plano original pedia para não tratar quase-duplicatas como pares isolados.

Para eficiência, cada grupo de mesmo hash é conectado em estrela (todos ligados
ao primeiro elemento do grupo) em vez de todos-com-todos — para fins de
conectividade dá no mesmo e evita adicionar arestas redundantes.

A saída no terminal segue a mesma organização de `deduplicacao_exata.py`
(`run()` com `print_section()` e seções numeradas: manifesto de entrada →
construção do grafo → componentes conexos → casos prioritários → caso
exploratório → relatórios gerados → resumo final), e o script valida
`relative_path` (preenchido e único) e `split`/`classe` antes de montar o
grafo, pelo mesmo motivo que `deduplicacao_exata.py` valida: evitar que uma
inconsistência silenciosa no manifesto vire uma contagem errada mais adiante.

Gera três relatórios:
- `resumo_componentes_t0.csv`: uma linha por componente com 2+ imagens (607
  no total), com contagem de imagens, quantos splits/classes ele mistura,
  ordenado para priorizar os casos com classes diferentes e depois os que
  atravessam `train`/`test`. A seção "Casos prioritários" do terminal mostra
  os 10 primeiros dessa mesma ordenação — calculados dinamicamente a cada
  execução, não fixados a um id específico.
- `imagens_por_componente_t0.csv`: cada imagem com o id do componente a que
  pertence, já com os metadados do manifesto.
- `componente_te_gl_68_t0.csv`: caso exploratório específico — o componente
  que contém `Testing/glioma/Te-gl_68.jpg`, a imagem que aparecia em 5 dos 6
  pares com classe diferente detectados em T=0.

### O que os números mostram

- 607 componentes com 2 ou mais imagens, somando 1.815 imagens envolvidas.
- Só **um** componente do dataset inteiro mistura classes diferentes:
  `t0_0020`, com 7 imagens, 2 classes (`glioma` e `meningioma`) e 2 splits
  (`train` e `test`) — é o componente de `Te-gl_68.jpg`, e é o candidato
  prioritário para a inspeção visual da Etapa 3.
- O maior componente é `t0_0134`, com 39 imagens, mesma classe mas
  atravessando `train`/`test` — vale olhar em seguida, por tamanho.
- 308 dos 607 componentes atravessam `train`/`test` sem conflito de classe —
  a maior parte do sinal de "vazamento perceptual" que a Etapa 2 levantou.

Nenhuma dessas imagens foi removida, convertida ou reclassificada — a coluna
`decision` em `componente_te_gl_68_t0.csv` está deliberadamente marcada como
`"pendente"`.

## Próximos passos (conforme o plano)

- Inspeção visual do componente `t0_0020` (o único com classes diferentes) e,
  por tamanho, do `t0_0134`.
- Definir e documentar o critério do T final (a análise de sensibilidade já
  está pronta; falta a decisão a priori).
- Registrar as decisões de curadoria (Etapa 3), seguindo o protocolo
  human-in-the-loop: vazamento remove a cópia de `test`; duplicata interna
  mantém a de melhor qualidade; conflito de rótulo é documentado e excluído
  por ambiguidade, nunca reclassificado.
