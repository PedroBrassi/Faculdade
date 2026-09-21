# Plano - Auditoria e Deduplicação da Base

Diário de bordo / plano de desenvolvimento. Organização revisada conforme as Etapas 0 a 3. Regras técnicas da versão de 04/09/2026 preservadas (correções em relação à primeira versão: split `val` removido da auditoria, BK-Tree retirado do escopo, checkpoint ajustado ao tamanho real da base, e regra de rótulos conflitantes trocada de "reclassificar" para "documentar e excluir"). Revisão de 20/09/2026: status das Etapas 2 e 3 atualizado com os resultados já executados.

## Etapa 0 - Ambiente e PoC

**Ambiente Python: concluído.** Ambiente virtual criado e bibliotecas `pillow`, `imagehash`, `pandas`, `networkx` e `matplotlib` instaladas. O uso de `pyarrow` é opcional, apenas se for usar Parquet.

**Prova de Conceito (PoC): concluída nas 100 imagens.** Foi desenvolvido o script `poc_auditoria.py` com 100 imagens de uma única classe do conjunto de treino, validando a leitura e integridade das imagens, a extração de metadados, o cálculo de SHA-256, pHash e dHash, a organização dos dados com pandas e o salvamento do arquivo de saída.

## Etapa 1 - Auditoria da Base

### Auditoria extensiva e geração do manifesto

**Concluída na base completa (7.200 imagens, `train` + `test`).** `auditoria.py` gerou `manifesto_base_dados.csv` com 0 falhas de leitura, 0 registros sem dimensão válida e hashes preservados após salvar/reabrir o CSV. Quatro arquivos `.jpg` da classe `notumor` (treino) foram identificados pelo Pillow como `PNG` — documentado em `divergencias_formato.csv`, arquivos mantidos como estão.

**Objetivo:** garantir a integridade física de cada arquivo, coletar metadados fundamentais e criar a fonte da verdade da base.

**Metadados obrigatórios por imagem:**
- `file_path`: caminho absoluto; `relative_path`: caminho relativo ao dataset.
- `split_assigned`: split de origem (`train`/`test`) — a base bruta não tem `val`; o split de validação só existe depois do resplit 70/15/15, que é uma etapa posterior, não da auditoria.
- `class_label`: subpasta ou rótulo atribuído.
- `file_size_bytes`: tamanho no disco em bytes.
- `width` e `height`: largura e altura em colunas separadas no CSV.
- `aspect_ratio`: razão de aspecto `(width / height)`.
- `color_mode`: modo de cor lido via Pillow (`RGB`, `L`, `RGBA`, `CMYK`).
- `channels`: número de canais (ex.: 3 para RGB, 1 para escala de cinza).
- `file_extension`: extensão do arquivo de origem (ex.: `.png`, `.jpg`).
- `is_valid`: booleano indicando sucesso do processamento implementado; não certifica ausência de duplicatas nem correção de rótulos.
- `error_flag`: mensagem tratada de erro (ex.: "Truncated file", "Cannot identify image file").
- `image_format`: formato real identificado pelo Pillow, independente da extensão do arquivo.

**Tratamento de exceções & tolerância a falhas:**
- Envolver o carregamento de cada imagem em um bloco `try/except` refinado (`PIL.UnidentifiedImageError`, `OSError`).
- Ativar `ImageFile.LOAD_TRUNCATED_IMAGES = True` com aviso, para identificar imagens parcialmente corrompidas. **Não implementado no código atual** (ver nota de implementação mais abaixo) — o código usa a leitura estrita padrão do Pillow, e não houve falha de leitura na base inteira que exigisse essa tolerância até agora.
- Checkpoint de progresso: **opcional** nessa base — o processamento completo das ~7.200 imagens leva ~1-2 minutos, então salvar a cada 5.000 imagens não cumpre função real (dispararia só uma vez, perto do fim). Implementado como mensagem de progresso a cada 500 arquivos, sem checkpoint em disco.

## Etapa 2 - Deduplicacao e Sensibilidade

### Deduplicação e análise de vazamento de dados

**Deduplicação exata SHA-256: concluída.** 153 grupos com hash repetido, 340 arquivos envolvidos, 187 cópias excedentes teóricas. Nenhum grupo atravessa `train`/`test` nem mistura classes.

**Análise de sensibilidade perceptual (pHash/dHash): concluída.** Tabulada nos limiares T ∈ {0, 3, 5, 8, 10} para os dois hashes. Em T=0: 1.930 pares por pHash (698 entre splits, 2 entre classes) e 2.776 pares por dHash (1.031 entre splits, 6 entre classes). A fila de revisão em T=0 (pHash **ou** dHash iguais, com split **ou** classe diferentes) reúne 1.236 pares candidatos, sendo 1.233 entre splits e 6 entre classes (3 pares com ambas as condições).

**Agrupamento em componentes conexos: concluído.** `componentes_conexos.py` liga imagens com pHash igual ou dHash igual num grafo e usa `networkx.connected_components` para capturar cadeias transitivas (A~B~C mesmo sem A~C direto). Resultado: 607 componentes com 2 ou mais imagens, somando 1.815 imagens; 308 componentes atravessam `train`/`test`; **apenas 1 componente do dataset inteiro mistura classes diferentes** (o do caso `Te-gl_68.jpg`, 7 imagens, glioma + meningioma, train + test); o maior componente (`t0_0134`) tem 39 imagens, mesma classe, atravessando `train`/`test`.

**Critério de decisão para o T final: ainda pendente.** A análise de sensibilidade já foi executada e documentada; falta definir e registrar a regra de escolha do T final antes de aplicá-la (ver nota abaixo sobre não descrever essa escolha retroativamente como se fosse uma regra a priori).

**Deduplicação exata:**
- Calcular o hash SHA-256 diretamente no buffer de bytes do arquivo de cada imagem no manifesto.
- Mapear conflitos exatos dentro do mesmo split e, prioritariamente, entre os splits de `train` e `test`.

**Deduplicação perceptual (quase-duplicatas):**
- Gerar hashes perceptuais usando `imagehash.phash()` e `imagehash.dhash()`.
- Comparar pares por distância de Hamming usando uma matriz de distância vetorizada (`scipy.spatial.distance.pdist`, métrica `hamming`).
  - **Nota sobre escala:** com ~7.200 imagens (~26 milhões de pares), essa abordagem resolve em segundos. Uma estrutura de indexação como BK-Tree não é necessária aqui — só valeria a pena se a base crescesse para centenas de milhares de imagens. Manter fora do escopo por ora.

**Análise de sensibilidade de limiares:**
- Testar múltiplos limiares de distância de Hamming (T ∈ {0, 3, 5, 8, 10}).
- Para cada limiar, tabular:
  - Total de pares detectados.
  - *Data Leakage Pairs*: candidatos entre `train` e `test`; vazamento ainda não confirmado visualmente.
  - *Label Mismatch Pairs*: pares sinalizados pelo hash com classes diferentes; semelhança e conflito precisam de inspeção.
- **Critério de decisão para o T final** (faltava na primeira versão): definir a regra a priori, por exemplo — maior T tal que a taxa de pares "classes diferentes" não ultrapasse um limite aceitável, sinal de que o hash ainda não está capturando ruído/falso positivo. Documentar a justificativa escolhida na metodologia.

**Agrupamento em componentes conexos:**
- Modelar as similaridades como um grafo não-dirigido (imagens = vértices, similaridade ≤ T = aresta).
- Usar busca em largura/profundidade (BFS/DFS) — por exemplo `networkx.connected_components` — para agrupar duplicatas em componentes conexos, não apenas pares isolados.
- Exemplo: se A é quase igual a B, e B é quase igual a C, então A, B e C formam um único cluster de duplicatas.

## Etapa 3 - Curadoria Visual e Decisao

### Curadoria visual e protocolo de exclusão

**Status: iniciada, parcial.** Os módulos de inspeção foram desenvolvidos como scripts (não como notebook único) e cobrem, até agora, dois casos específicos em vez do dataset completo de componentes:
- O único componente com classes diferentes (`t0_0020`, caso `Te-gl_68.jpg`): comparação de pixels RGB entre pares do componente, incluindo teste de inversão vertical e rotação de 180°.
- O maior componente (`t0_134`, 39 imagens): agrupamento por igualdade exata de pixels RGB (achou 2 subgrupos de pixels idênticos apesar de SHA-256 diferentes) e comparação par a par por rotação (0°/90°/180°/270°) entre os 32 representantes distintos.

**Pendente:** estender a inspeção aos demais 605 componentes (ou definir um critério de priorização documentado para não revisar todos manualmente), e decidir o formato final do log de decisões.

**Atenção — confirmação humana ainda não está garantida no registro do caso `Te-gl_68.jpg`.** O script que gera a decisão desse caso (`registrar_decisao.py`) grava `decision = "excluir_da_base_curada"` com a justificativa "confirmada pelo responsável" de forma incondicional — o código não captura nenhuma confirmação real de quem executa o script (sem exibir as imagens, sem entrada manual). Antes de tratar esse CSV como decisão final documentada na metodologia, confirme que a revisão das 7 imagens de fato aconteceu; caso contrário, o registro deveria voltar para `"pendente"` até a revisão real ocorrer, do mesmo jeito que os outros casos já inspecionados (`revisao_te_no_113_te_no_140.csv`, `revisao_te_no_271_tr_no_611.csv`, `revisao_inicial_classes_diferentes.csv`) corretamente permanecem pendentes.

**Módulo de inspeção visual:**
- Visualizador (Matplotlib/Plotly) para renderizar todos os elementos de um cluster conexo lado a lado.
- Exibir sobre cada imagem: caminho, split, classe e hash (SHA-256/pHash).
- Destacar visualmente (ex.: borda vermelha) qualquer par que viole a regra de split (`train` vs `test`) ou de classe.

**Diretrizes de exclusão manual (human-in-the-loop):**
- Nenhuma imagem é deletada via script automatizado sem confirmação manual do inspetor.
- Regras de decisão:
  - **Vazamento de dados (train x test):** remover a versão de `test` ou descartar a duplicata do `train`.
  - **Duplicata interna no mesmo split:** manter apenas a imagem de maior resolução/melhor qualidade.
  - **Rótulos conflitantes** (revisado): **não** reclassificar a imagem por conta própria — não há base clínica para corrigir um diagnóstico de um dataset curado. Em vez disso, **documentar o caso e excluir o par do conjunto por ambiguidade**.

---

## Lista de Tarefas Ordenada (ToDo)

### Etapa 0 - Ambiente e PoC
- [X] Criar ambiente virtual Python e instalar `pillow`, `imagehash`, `pandas`, `networkx`, `matplotlib` (`pyarrow` só se for usar Parquet — opcional para este tamanho de base).
- [X] Escrever script PoC em 100 imagens de uma única classe do conjunto de treino para validar leitura e integridade das imagens, extração de metadados, SHA-256, pHash/dHash, organização com pandas e salvamento do arquivo de saída.

### Etapa 1 - Auditoria da Base
- [X] Rodar o script de auditoria no dataset completo (`train` e `test`).
- [X] Exportar o manifesto consolidado (`manifesto_base_dados.csv` ou `.parquet`).
- [X] Gerar relatório sumário de erros (imagens corrompidas, arquivos sem dimensão válida, modos de cor atípicos).

### Etapa 2 - Deduplicacao e Sensibilidade
- [X] Calcular SHA-256 para todas as entradas do manifesto e agrupar duplicatas exatas.
- [X] Calcular `pHash` e `dHash` para todas as imagens válidas.
- [X] Rodar a matriz de sensibilidade com limiares T ∈ {0, 3, 5, 8, 10} e exportar os relatórios de Data Leakage e Label Mismatch.
- [ ] Definir e documentar o critério de decisão para o T final.
- [X] Implementar o agrupamento por componentes conexos (`networkx.connected_components`) para consolidar clusters de duplicatas — 607 componentes com 2+ imagens, 1 com classes diferentes.

### Etapa 3 - Curadoria Visual e Decisao
- [ ] Desenvolver notebook de inspeção visual focado em clusters com conflito de split (`train` x `test`) e de classe. *(Parcial: scripts avulsos cobrem o componente com classes diferentes e o maior componente; os demais 605 componentes ainda não foram inspecionados.)*
- [ ] Gerar arquivo de log/decisão final (`exclusoes_e_ajustes.csv`) contendo os caminhos a remover/remanejar e o motivo de cada decisão. *(Existe um registro para o caso `Te-gl_68.jpg` — `decisao_te_gl_68_t0.csv` — mas precisa de confirmação humana real antes de contar como decisão final; ver nota acima. Falta também generalizar o formato para os demais casos.)*
- [ ] Executar o expurgo definitivo e gerar o manifesto final higienizado.

---

*Board Trello correspondente: [TCC - Classificação de Tumores Cerebrais](https://trello.com/b/OJmQifd0/tcc-classifica%C3%A7%C3%A3o-de-tumores-cerebrais)*

## Como interpretar os resultados atuais

- Grupo SHA-256: conjunto de arquivos com o mesmo hash exato. Não confundir com pares.
- Par candidato: combinação de duas imagens sinalizada por hash perceptual.
- Par sem repetição: A-B aparece uma vez, mesmo se pHash e dHash sinalizarem o par. Não significa imagem sem duplicação.
- T=0: igualdade do hash perceptual, não confirmação de duplicação.
- Os limiares são cumulativos; não somar suas contagens. Classes diferentes não equivalem a falsos positivos confirmados.
- O relatório T=0 usa (pHash igual OU dHash igual) E (split diferente OU classe diferente).
- Componente conexo: grupo de imagens ligadas direta ou indiretamente por hash igual (T=0). Duas imagens no mesmo componente podem não ter hash igual entre si — a ligação pode vir de uma imagem intermediária.
- `visual_assessment`/`observation` nos CSVs da Etapa 3: só valem como avaliação real quando o script efetivamente exibiu as imagens **e** uma pessoa registrou a conclusão. Um texto de avaliação por si só não é prova de que a inspeção aconteceu.
- O script calcula candidatos; inspeção humana completa, T final e expurgo definitivo continuam pendentes.

Nota de implementação: o código atual usa a leitura estrita padrão do Pillow, sem ativar LOAD_TRUNCATED_IMAGES. A regra histórica acima não está implementada; ativar leitura tolerante não equivale a detectar corrupção. Esta revisão de clareza não alterou esse comportamento.

A sensibilidade já foi explorada. Qualquer escolha futura de T deve documentar essa exploração; não deve ser descrita retroativamente como uma regra a priori.
