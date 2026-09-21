# Etapa 1 — Auditoria da Base

## Objetivo

Percorrer toda a base (`Training` e `Testing`), extrair metadados de cada
imagem e calcular SHA-256, pHash e dHash, gerando o manifesto que serve de
entrada para a Etapa 2 (deduplicação e análise de sensibilidade). Falhas de
leitura são registradas, não interrompem o processamento das demais imagens.

## Conteúdo desta pasta

```
Etapa 1 - Auditoria da Base/
├── auditoria.py
└── resultados/
    ├── manifesto_base_dados.csv     (todas as imagens processadas)
    ├── relatorio_erros.csv          (só as que falharam — vazio nesta execução)
    ├── revisao_modos_cor.csv        (imagens em modo P ou RGBA)
    └── divergencias_formato.csv     (extensão do arquivo ≠ formato lido pelo Pillow)
```

## Como executar

```bash
python auditoria.py
```

Pré-requisitos: ambiente virtual com `pillow`, `imagehash` e `pandas`
instalados. O script localiza a raiz do projeto a partir de si mesmo
(`Path(__file__).resolve().parents[2]`) e espera encontrar o dataset em
`<raiz do projeto>/Brain Tumor MRI Dataset/{Training,Testing}`. Se o script
for movido para outra profundidade de pastas, esse índice precisa ser
ajustado.

## O que o script faz

1. **Inventário**: lista os arquivos `.jpg`/`.jpeg`/`.png` de `Training` e
   `Testing`, mapeados para `train`/`test` (não existe `val` nesta etapa —
   isso é decidido depois, na preparação dos dados). Mostra a contagem por
   split e classe antes de processar.
2. **Processamento**: para cada imagem, calcula tamanho em bytes, SHA-256,
   largura, altura, modo de cor, canais, aspect ratio, pHash, dHash e o
   formato identificado pelo Pillow (`image_format`). Progresso é impresso a
   cada 500 arquivos.
3. **Exportação e conferência**: salva `manifesto_base_dados.csv` e
   `relatorio_erros.csv`, depois reabre o manifesto (hashes como texto, para
   não perder zeros à esquerda) e confere se as dimensões da tabela e os
   valores dos hashes batem com o que foi gerado antes de salvar. Se não
   baterem, o script interrompe com erro em vez de seguir com dados
   suspeitos.
4. **Perfil da base**: distribuição por split/classe, por modo de
   cor/canais, as 10 dimensões mais frequentes, e checagem de registros sem
   dimensão válida.
5. **Sinalizações para revisão**: exporta imagens em modo `P`/`RGBA` (podem
   precisar de conversão para RGB antes do treinamento) e arquivos cuja
   extensão não bate com o formato real detectado pelo Pillow. Nenhum
   arquivo é convertido, renomeado ou excluído — é só documentação para a
   curadoria.

## Colunas do manifesto

| Coluna | Significado |
|---|---|
| `file_path` | Caminho absoluto no momento da execução. |
| `relative_path` | Caminho relativo à pasta do dataset — estável entre máquinas/pastas diferentes. |
| `class_label` | `glioma`, `meningioma`, `notumor` ou `pituitary`. |
| `file_extension` | Extensão original do arquivo. |
| `split_assigned` | `train` (de `Training`) ou `test` (de `Testing`). |
| `file_size_bytes` | Tamanho do arquivo em disco. |
| `sha256_hash` | Hash exato do conteúdo do arquivo. |
| `width`, `height` | Dimensões em pixels. |
| `color_mode`, `channels` | Modo de cor do Pillow e número de canais. |
| `aspect_ratio` | `width / height`. |
| `phash`, `dhash` | Hashes perceptuais (64 bits, formato hexadecimal). |
| `is_valid` | `True` se a leitura e os hashes terminaram sem erro. **Não** indica ausência de duplicatas nem rótulo correto. |
| `error_flag` | Mensagem de erro capturada, quando houver. |
| `image_format` | Formato real do arquivo, identificado pelo Pillow (ex.: `JPEG`, `PNG`), independente da extensão. |

## Resultado da última execução registrada

- 7.200 imagens processadas (5.600 em `train`, 1.600 em `test`; 1.400/400 por classe).
- 0 falhas de leitura — `relatorio_erros.csv` ficou só com o cabeçalho.
- 0 registros sem dimensão válida.
- Dimensões da tabela e hashes preservados após salvar/reabrir o CSV.
- 4 imagens em modo `P`/`RGBA` (todas na classe `notumor`, treino) — ver `revisao_modos_cor.csv`.
- 4 divergências entre extensão e formato: são os mesmos 4 arquivos acima —
  todos têm extensão `.jpg` mas foram identificados como `PNG` pelo Pillow.
  Puderam ser lidos normalmente; a divergência foi documentada, não corrigida.

> **Nota:** os caminhos absolutos (`file_path`) nestes CSVs foram gravados
> com a raiz `C:\Users\pedro\Desktop\Projeto do TCC\...`, de uma execução
> feita em outra cópia/pasta do projeto. Não afeta a lógica do script — o
> `relative_path` já é a coluna preparada para não depender dessa raiz — mas
> os valores de `file_path` vão mudar assim que o script rodar de novo na
> pasta atual.

## Escopo

Esta etapa não deduplica, não decide exclusões e não altera nenhuma imagem.
Isso é feito na Etapa 2 (deduplicação exata e análise de sensibilidade dos
hashes perceptuais) e na Etapa 3 (curadoria visual e decisão).
