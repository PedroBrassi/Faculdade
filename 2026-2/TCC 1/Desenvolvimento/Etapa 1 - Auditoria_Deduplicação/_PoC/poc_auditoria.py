"""
PoC (Prova de Conceito) - Etapa 0 do plano.

Objetivo: validar, em 100 imagens de UMA classe, cada peça do pipeline antes
de rodar na base inteira (~7.200 imagens):
    1. extração de metadados basicos (dimensao, modo de cor, tamanho em bytes)
    2. hash SHA-256 (do arquivo em disco)
    3. hash perceptual pHash (da imagem decodificada)
    4. tratamento de erro em arquivo invalido/corrompido
    5. montagem e salvamento de uma tabela (DataFrame) com os resultados

Preencha os TODOs. Nao precisa (e nao deve) ser o script final -- e so a
prova de que cada peca funciona antes de escalar.
"""

import hashlib
from pathlib import Path

import pandas as pd
from PIL import Image
import imagehash


# --- Configuracao ----------------------------------------------------------

# TODO: aponte para UMA classe dentro de Training (ex.: "glioma").
# Dica: a base fica em "Brain Tumor MRI Dataset - A dataset for classify
# brain tumors/Training/<classe>". Use caminho relativo a partir desta pasta,
# ou absoluto -- o que for mais facil de conferir por enquanto.
CLASS_DIR = Path("???")

SAMPLE_SIZE = 100

OUT_CSV = Path(__file__).resolve().parent / "poc_manifest.csv"


# --- Funcoes a implementar ---------------------------------------------------

def compute_sha256(path: Path) -> str:
    """
    Calcula o hash SHA-256 do CONTEUDO BINARIO do arquivo em disco (nao da
    imagem decodificada -- e o arquivo, byte a byte).

    Dica:
        - hashlib.sha256() cria o objeto de hash.
        - abra o arquivo em modo binario ('rb') e alimente o hash com os
          bytes lidos.
        - .hexdigest() no final devolve a string do hash.

    TODO: implementar.
    """
    raise NotImplementedError


def extract_image_metadata(path: Path) -> dict:
    """
    Abre a imagem com Pillow e extrai:
        - width, height (de .size)
        - mode (modo de cor, ex. "RGB", "L")
        - phash (imagehash.phash(imagem_aberta))

    Atencao: Image.open() e "lazy" -- os dados da imagem so sao
    efetivamente lidos quando voce acessa algo como .size ou passa a
    imagem para uma funcao que precise dos pixels (como o phash).

    Deve devolver um dicionario, por exemplo:
        {"width": ..., "height": ..., "mode": ..., "phash": ...}

    TODO: implementar.
    """
    raise NotImplementedError


def audit_one_image(path: Path) -> dict:
    """
    Junta tudo para UMA imagem:
        - metadados de arquivo: tamanho em bytes (path.stat().st_size) e
          extensao (path.suffix)
        - sha256 (chame compute_sha256)
        - metadados de imagem (chame extract_image_metadata)

    IMPORTANTE: envolva a parte que abre/decodifica a imagem em um
    try/except (PIL.UnidentifiedImageError, OSError). Se der erro, devolva
    um dicionario com is_valid=False e error_flag preenchido com a
    mensagem -- NAO deixe a excecao propagar e derrubar o script.

    Retorno esperado (um dict por imagem), por exemplo:
        {
            "path": str(path),
            "file_size_bytes": ...,
            "file_extension": ...,
            "is_valid": True ou False,
            "error_flag": "" ou mensagem de erro,
            "width": ..., "height": ..., "mode": ..., "phash": ...,
            "sha256": ...,
        }
    (quando is_valid=False, os campos de imagem podem ficar None)

    TODO: implementar, chamando as duas funcoes acima dentro do try.
    """
    raise NotImplementedError


def run_poc():
    """
    1. Lista ate SAMPLE_SIZE arquivos de CLASS_DIR (Path.glob + slice).
    2. Chama audit_one_image para cada um.
    3. Junta os resultados numa lista de dicts -> pd.DataFrame.
    4. Salva em OUT_CSV (df.to_csv).
    5. Imprime um resumo: quantas imagens validas, quantas com erro.

    TODO: implementar.
    """
    raise NotImplementedError


if __name__ == "__main__":
    run_poc()


# --- Depois de implementar, teste manualmente (nao precisa virar codigo) ---
# 1. compute_sha256 no mesmo arquivo duas vezes -> tem que dar o mesmo hash.
# 2. Copie uma imagem, redimensione a copia, calcule o phash das duas ->
#    devem ficar PARECIDOS mas nao necessariamente identicos.
# 3. Renomeie um .txt para .jpg numa pasta de teste e rode audit_one_image
#    nele -> confirme que cai no except, sem travar o script.
