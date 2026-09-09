"""
PoC (Prova de Conceito) - Etapa 0.

Valida, em 100 imagens de uma única classe, a extração de metadados, o
hash SHA-256 (duplicata exata) e os hashes perceptuais phash/dhash
(quase-duplicata), antes de rodar na base inteira (Etapa 1).
"""

import hashlib
from pathlib import Path

import imagehash
import pandas as pd
from PIL import Image, UnidentifiedImageError

# --- Configuração ------------------------------------------------------------

VALID_EXTENSIONS = {".jpg", ".jpeg", ".png"}
SAMPLE_SIZE = 100

# parents[3]: _PoC -> "Etapa 0 - Ambiente e Poc" -> Desenvolvimento -> TCC 1.
# Ajuste o índice se mover este script para outra profundidade.
PROJECT_ROOT = Path(__file__).resolve().parents[3]
CLASS_DIR = PROJECT_ROOT / "Brain Tumor MRI Dataset" / "Training" / "meningioma"

OUTPUT_DIR = Path(__file__).resolve().parent / "resultados"
OUTPUT_PATH = OUTPUT_DIR / "manifesto_poc.csv"


# --- Funções ------------------------------------------------------------------

def list_sample_images(class_dir: Path, sample_size: int) -> list[Path]:
    """Lista as imagens da classe, filtra por extensão válida e retorna as
    primeiras `sample_size`, ordenadas por nome (seleção reprodutível, não
    aleatória -- adequada para uma PoC)."""
    images = [
        item for item in class_dir.iterdir()
        if item.is_file() and item.suffix.lower() in VALID_EXTENSIONS
    ]
    return sorted(images)[:sample_size]


def compute_sha256(path: Path) -> str:
    """Hash SHA-256 do conteúdo binário do arquivo em disco."""
    with path.open("rb") as f:
        return hashlib.sha256(f.read()).hexdigest()


def extract_image_metadata(image: Image.Image) -> dict:
    """Metadados derivados da imagem já aberta/decodificada."""
    return {
        "width": image.width,
        "height": image.height,
        "color_mode": image.mode,
        "aspect_ratio": image.width / image.height,
        "channels": len(image.getbands()),
        "phash": str(imagehash.phash(image)),
        "dhash": str(imagehash.dhash(image)),
    }


def build_record(image_path: Path) -> dict:
    """Monta um registro completo para uma imagem: metadados de arquivo,
    sha256 e metadados de imagem. Nunca propaga exceção -- imagem inválida
    vira um registro com is_valid=False."""
    record = {
        "file_path": str(image_path),
        "class_label": image_path.parent.name,
        "file_extension": image_path.suffix.lower(),
        "split_assigned": "train",  # fixo: esta PoC usa apenas Training.
        "file_size_bytes": None,
        "sha256_hash": None,
        "width": None,
        "height": None,
        "color_mode": None,
        "aspect_ratio": None,
        "channels": None,
        "phash": None,
        "dhash": None,
        "is_valid": False,
        "error_flag": None,
    }
    try:
        record["file_size_bytes"] = image_path.stat().st_size
        record["sha256_hash"] = compute_sha256(image_path)

        with Image.open(image_path) as image:
            image.load()  # força a decodificação (pega arquivo truncado)
            record.update(extract_image_metadata(image))

        record["is_valid"] = True

    except (UnidentifiedImageError, OSError) as error:
        print(f"Erro ao ler a imagem {image_path.name}: {error}")
        record["error_flag"] = str(error)

    return record


def verify_round_trip(df: pd.DataFrame, csv_path: Path) -> bool:
    """Confirma que salvar e reler o CSV preserva os hashes (evita
    surpresa de tipo/precisão ao reabrir os dados depois)."""
    reloaded = pd.read_csv(
        csv_path,
        encoding="utf-8-sig",
        dtype={"sha256_hash": "string", "phash": "string", "dhash": "string"},
    )
    print(f"Dimensões da tabela reaberta: {reloaded.shape}")

    hash_columns = ["sha256_hash", "phash", "dhash"]
    preserved = df[hash_columns].astype("string").equals(reloaded[hash_columns])
    print("Hashes preservados:", preserved)
    return preserved


def run():
    OUTPUT_DIR.mkdir(exist_ok=True)

    sample_paths = list_sample_images(CLASS_DIR, SAMPLE_SIZE)
    records = [build_record(path) for path in sample_paths]

    df = pd.DataFrame(records)
    print(df["is_valid"].value_counts())

    df.to_csv(OUTPUT_PATH, index=False, encoding="utf-8-sig")
    print(f"Manifesto salvo em: {OUTPUT_PATH}")

    verify_round_trip(df, OUTPUT_PATH)


if __name__ == "__main__":
    run()