#!/bin/bash

EXEC="$(dirname "$0")/base"
DATA="$(dirname "$0")/data"
OUT="$(dirname "$0")/results.txt"

> "$OUT"
COUNT=1

shopt -s nullglob

for FILE in "$DATA"/*.txt
do
    echo "===== Teste $COUNT =====" >> "$OUT"
    echo "Arquivo: $FILE" >> "$OUT"

    "$EXEC" "$FILE" >> "$OUT"

    echo "" >> "$OUT"

    COUNT=$((COUNT + 1))
done