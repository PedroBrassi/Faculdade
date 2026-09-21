# Curadoria de duplicatas — Brain Tumor MRI Dataset

Pipeline único de auditoria e deduplicação para o TCC "Classificação de
Tumores Cerebrais em Imagens de Ressonância Magnética Utilizando
Transferência de Aprendizado" (Pedro Brassi Luccas, UNIFAL).

Este projeto **substitui** os dois pipelines que existiam em paralelo
(um baseado em `interface_botoes.py` e nos scripts soltos de
`Etapa 2`/`Etapa 3`; outro no pacote `curadoria-mri` com canonicalização
diédrica). Ele junta o melhor de cada um:

- A canonicalização por rotação/reflexão (grupo diédrico D4) e o índice
  de Hamming multi-tabela do pacote `curadoria-mri` — testados, exatos,
  mais rigorosos que a comparação por MAE tentando 4 rotações.
- A ferramenta de revisão manual com botões (antes `interface_botoes.py`)
  — único jeito de avaliar pares na base, mantido quase igual.
- As **221 revisões manuais já feitas** por Pedro no componente `t0_0134`
  — migradas, não descartadas (`migracao/`).

## Por que isso mudou em relação às duas versões anteriores

O pré-projeto de TCC (seção "Auditoria, deduplicação e divisão dos
dados") é explícito:

> "Pares serão registrados, agrupados e inspecionados por amostragem,
> **sem exclusão automática** de imagens semelhantes, pois podem
> representar cortes distintos de um mesmo exame."

A versão do pacote `curadoria-mri` tinha uma coluna `confirmed` decidida
sozinha por `ssim >= 0.85`, que alimentava um arquivo chamado
`grupos_confirmados.csv` com ~1900 imagens — sem nenhuma revisão humana.
Isso contradizia diretamente o pré-projeto. Neste projeto:

- Não existe mais nenhuma coluna "confirmed"/"confirmado" calculada
  automaticamente. O que existe é `sugestao_revisao`: uma referência
  textual para orientar o inspetor, igual à antiga função `sugerir_relacao`.
- O agrupamento (`cluster.py`) parte de TODOS os pares candidatos da
  camada 3 (estrutural — quem é candidato de quem), não de um
  subconjunto pré-filtrado por limiar.
- A decisão final de excluir uma imagem só existe em `decisao.py`, e
  **exige** um `relative_path`, uma `decision` (`mantida`/`excluida`) e um
  `reason` em texto livre — nada é derivado automaticamente de SSIM ou do
  rótulo que a pessoa deu ao par. Sem isso, o comando falha.

## Estrutura

```
curadoria/
  canonical.py   # camada 2: hash canônico invariante a rotação/reflexão (D4)
  index.py       # índice de Hamming multi-tabela (busca aproximada em O(1) amortizado)
  detect.py      # camadas 1 (SHA-256), 2 (canonical) e 3 (perceptual/candidatos)
  score.py       # MAE + SSIM por par candidato — só pontua, não decide
  cluster.py     # componentes conexos sobre os candidatos + ordenação de fila
  revisao.py     # janela interativa de revisão manual (botões)
  decisao.py     # único lugar que pode marcar uma imagem como excluída
  visualize.py   # figuras de grupos cross-class para o texto do TCC
  cli.py         # `python -m curadoria.cli <comando>` — ver abaixo
migracao/
  migrar_revisoes_antigas.py       # traz as 221 revisões manuais já feitas
  dados_legados/                   # cópia da fonte original dessas revisões
tests/
  test_canonical.py   # propriedades algébricas da canonicalização (herdado)
  test_cluster.py     # transitividade do agrupamento, flags cross-split/class
figuras/
  grupo_A_te_gl_68.png / grupo_B_te_gl_160.png   # os 2 vazamentos cross-class já achados
```

## Como rodar, do zero

```bash
pip install -r requirements.txt

# 1) Inventário: SHA-256 + hash canônico D4 + phash/dhash de cada imagem
python -m curadoria.cli inventory \
    --dataset "Brain Tumor MRI Dataset" \
    --output resultados/inventario.csv

# 2) Detecção em 3 camadas (T=4 é o padrão; ajuste com --max-distance)
python -m curadoria.cli detect \
    --inventory resultados/inventario.csv \
    --output resultados \
    --max-distance 4

# 3) Pontuação fina dos candidatos da camada 3 (MAE + SSIM, sem decidir nada)
python -m curadoria.cli score \
    --dataset "Brain Tumor MRI Dataset" \
    --pairs resultados/camada3_candidatos.csv \
    --output resultados/pontuados.csv

# 4) Agrupamento em componentes conexos
python -m curadoria.cli cluster \
    --pairs resultados/camada3_candidatos.csv \
    --inventory resultados/inventario.csv \
    --output resultados/clusters.csv

# 5) Controle mestre de curadoria (todas as imagens começam 'pendente')
python -m curadoria.cli init-controle \
    --inventory resultados/inventario.csv \
    --output resultados/controle_curadoria.csv

# 6) Trazer as 221 revisões manuais já feitas antes desta reestruturação
python -m migracao.migrar_revisoes_antigas \
    --legado migracao/dados_legados/t0_0134_revisoes_consolidadas.csv \
    --saida resultados/revisoes.csv \
    --clusters resultados/clusters.csv

# 7) Revisão manual interativa dos pares restantes (abre janela com botões)
python -m curadoria.cli revisar \
    --dataset "Brain Tumor MRI Dataset" \
    --pontuados resultados/pontuados.csv \
    --clusters resultados/clusters.csv \
    --saida resultados/revisoes.csv

# 8) Relatório por cluster para apoiar a decisão (não decide nada sozinho)
python -m curadoria.cli resumo-decisao \
    --revisoes resultados/revisoes.csv \
    --output resultados/resumo_para_decisao.csv

# 9) Decisão final — sempre um ato humano explícito, um comando por imagem
python -m curadoria.cli decidir \
    --controle resultados/controle_curadoria.csv \
    --relative-path "Testing/glioma/Te-gl_68.jpg" \
    --decision excluida \
    --reason "Duplicata visual confirmada com rótulo conflitante (ver figuras/grupo_A_te_gl_68.png)."

# ...ou em lote, a partir de um CSV que você mesmo preencheu (nunca gerado):
python -m curadoria.cli decidir-lote \
    --controle resultados/controle_curadoria.csv \
    --decisoes minhas_decisoes.csv

# Testes
pytest -v
```

## O que foi removido em relação às duas versões anteriores

Scripts com lógica hoje coberta pelo pacote `curadoria/` (produziam
resultado equivalente ou pior, com mais duplicação de código):
`componentes_conexos.py`, `deduplicacao_exata.py`, `comparar_rotacoes.py`,
`agrupar_pixels_rgb.py`, `agrupar_com_restricoes.py`,
`analisar_consistencia_grupos.py`, `inspecao_componentes.py`,
`consolidar_revisoes.py`, `registros_revisao.py`, `revisar_contradicoes.py`,
`revisar_fila.py`, `revisar_fila_anterior.py`, `revisar_inconclusivos.py`,
`separar_inconclusivos.py`, `tarefas.py`, `figuras_cross_class.py` (mantido
como script solto de apoio, não fazia parte do pacote).

Removidos por estarem incorretos, não só duplicados:
- `inspecao_visual.py` — escrevia `visual_assessment` e uma observação
  fixa para todos os pares do componente cross-class **sem nunca abrir
  uma imagem**.
- `registrar_decisao.py` / `controle_curadoria.py` (versões antigas) —
  escreviam `decision = "excluir_da_base_curada"` e
  `reason = "...confirmada pelo responsável"` incondicionalmente, sem
  nenhum mecanismo real de confirmação humana.
- A coluna `confirmed` do pacote `curadoria-mri` (ver seção acima).

Preservado como dado, não como código: as 221 avaliações manuais reais
(cliques de botão, não texto fabricado) migradas por
`migracao/migrar_revisoes_antigas.py`, e os dois achados de vazamento
cross-class (`figuras/`).

## Pendências conhecidas

- Os limiares de `score.py` (MAE_BAIXO, SSIM_ALTO etc.) ainda não foram
  calibrados estatisticamente contra as revisões manuais reais — são os
  mesmos valores provisórios de antes. Vale usar as 221 (agora 185
  migradas) revisões como conjunto de calibração antes de confiar na
  `sugestao_revisao` em escala.
- A divisão treino/validação/teste por paciente/grupo de duplicatas
  (pré-projeto, seção Metodologia) ainda não está implementada neste
  pipeline — ele cobre auditoria e curadoria, não a divisão final dos
  dados nem as etapas de treinamento (E1–E7).
