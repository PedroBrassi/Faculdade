# Classificação de Tumores Cerebrais em Imagens de Ressonância Magnética

Projeto desenvolvido como parte do Trabalho de Conclusão de Curso (TCC) em Ciência da Computação.

O objetivo do trabalho é realizar a classificação de tumores cerebrais em imagens de ressonância magnética utilizando técnicas de **Transfer Learning**, além de comparar os modelos considerando:

- desempenho na classificação;
- eficiência computacional;
- explicabilidade dos resultados.

## Estrutura do Projeto

O desenvolvimento foi dividido em diferentes etapas para facilitar a organização, validação e documentação dos experimentos.

```text
TCC 1/
│
├── Artigos/
│
├── Brain Tumor MRI Dataset/
│   ├── Training/
│   └── Testing/
│
├── Desenvolvimento/
│   ├── .venv/
│   ├── requirements.txt
│   │
│   ├── Etapa 0 - Ambiente e Poc/
│   │   └── _PoC/
│   │       └── poc_auditoria.py
│   │
│   ├── Etapa 1 - Auditoria_Deduplicação/
│   │   ├── Plano_1ª_Etapa.md
│   │   ├── auditoria.py
│   │   └── resultados/  (manifesto_base_dados.csv, relatorio_erros.csv, etc. -- gerados na execução)
│   │
│   └── Relatório Diário/
│
├── Material de Apoio - TCC/
│
├── Relatórios/
│
└── Pre-Projeto_TCC.pdf
```

## Etapa 1 — Auditoria e Deduplicação da Base

A primeira etapa do projeto tem como objetivo verificar a integridade da base de imagens antes do treinamento dos modelos.

Entre as atividades previstas estão:

- verificação da integridade dos arquivos;
- extração de metadados das imagens;
- geração de manifesto da base;
- identificação de duplicatas exatas utilizando SHA-256;
- identificação de imagens semelhantes utilizando pHash e dHash;
- análise de possíveis vazamentos entre os conjuntos de treino e teste;
- identificação de conflitos entre imagens semelhantes com classes diferentes;
- agrupamento de duplicatas utilizando grafos;
- inspeção visual e curadoria dos casos encontrados.

Antes da execução na base completa foi realizada uma **Prova de Conceito (PoC)** utilizando 100 imagens de uma única classe, para validar a extração de metadados e os hashes antes de escalar para o dataset inteiro (~7.200 imagens, conjuntos `Training` e `Testing`).

## Tecnologias

O projeto está sendo desenvolvido utilizando Python.

Principais bibliotecas utilizadas atualmente:

- Pillow
- ImageHash
- pandas
- NetworkX
- Matplotlib
- SciPy

Outras bibliotecas relacionadas ao treinamento dos modelos de Deep Learning serão adicionadas posteriormente.

## Configuração do Ambiente

É recomendado utilizar um ambiente virtual Python.

### Criar o ambiente virtual

```bash
python -m venv .venv
```

### Ativar no Windows PowerShell

```powershell
.\.venv\Scripts\Activate.ps1
```

### Instalar as dependências

```bash
python -m pip install -r requirements.txt
```

## Dataset

O dataset utilizado no projeto **não está incluído neste repositório**.

Os arquivos da base são mantidos localmente e ignorados pelo Git por meio do arquivo `.gitignore`.

A estrutura esperada da base contém originalmente os conjuntos de treino e teste:

```text
Brain Tumor MRI Dataset/
├── Training/
│   └── classes...
│
└── Testing/
    └── classes...
```

O conjunto de validação será criado posteriormente durante a etapa de preparação dos dados.

## Desenvolvimento Atual

Atualmente o projeto encontra-se na:

**Etapa 1 — Auditoria e Deduplicação da Base**

Situação atual:

- [x] Configuração do ambiente Python
- [x] Criação do ambiente virtual
- [x] Instalação das dependências iniciais
- [x] Geração do `requirements.txt`
- [x] Prova de Conceito com 100 imagens (validada: arquivo inválido tratado, SHA-256 determinístico, pHash robusto a redimensionamento — distância 0 vs. 28 no controle)
- [x] Auditoria da base completa (script `auditoria.py` escrito, extraindo metadados, SHA-256, pHash/dHash e relatórios de erro/divergência para `Training` + `Testing`; falta rodar na base completa e conferir a saída)
- [ ] Deduplicação
- [ ] Análise de vazamento de dados
- [ ] Curadoria visual
- [ ] Geração do manifesto higienizado

## Autor

Pedro Brassi Luccas
Ciência da Computação — Universidade Federal de Alfenas (UNIFAL-MG)
