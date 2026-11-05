# Base de Dados

## Identificação

| Campo | Valor |
|---|---|
| Nome | Brain Tumor MRI Dataset |
| Autor | Masoud Nickparvar |
| URL | https://www.kaggle.com/datasets/masoudnickparvar/brain-tumor-mri-dataset |
| Versão | 2 (atualização de 2026-02-13) |
| Data do download | 2026-09-23 |
| Licença | CC BY 4.0 |
| Local | `database/raw/` (não versionado no Git) |

## Fontes primárias

Segundo o autor, a base é uma combinação de três conjuntos públicos:

| Fonte | URL | Licença |
|---|---|---|
| figshare (Cheng et al.) | https://figshare.com/articles/dataset/brain_tumor_dataset/1512427 | CC BY 4.0 |
| SARTAJ (Bhuvaji et al.) | https://www.kaggle.com/datasets/sartajbhuvaji/brain-tumor-classification-mri | MIT |
| Br35H (Hamada) | https://www.kaggle.com/datasets/ahmedhamada0/brain-tumor-detection | copyright-authors |

## Conteúdo

7.200 imagens `.jpg` (175 MB), divididas em 4 classes:

| Classe | Training | Testing | Total |
|---|---:|---:|---:|
| glioma | 1.400 | 400 | 1.800 |
| meningioma | 1.400 | 400 | 1.800 |
| notumor | 1.400 | 400 | 1.800 |
| pituitary | 1.400 | 400 | 1.800 |
| **Total** | **5.600** | **1.600** | **7.200** |

Questões de qualidade, duplicatas e vazamento de dados são tratadas na etapa de auditoria da base.

## Referência

NICKPARVAR, Masoud. Brain Tumor MRI Dataset. Versão 2. Kaggle, 2026. Disponível em: https://www.kaggle.com/datasets/masoudnickparvar/brain-tumor-mri-dataset. Acesso em: 23 set. 2026.
