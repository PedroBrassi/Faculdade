#!/bin/bash

mkdir -p graphs

# tamanhos variados
sizes=(500 1000 2000 5000 10000)

# quantidade por tamanho
qtd=2   # 2 por tamanho → total 10 por topologia

# ---------------- ERDOS ----------------
for n in "${sizes[@]}"
do
  for ((i=1;i<=qtd;i++))
  do
    p=0.01
    filename="graphs/erdos-${n}-p${p}-${i}.txt"
    python generator.py erdos $n $p > "$filename"
  done
done

# ---------------- BARABASI ----------------
for n in "${sizes[@]}"
do
  for ((i=1;i<=qtd;i++))
  do
    m=3
    filename="graphs/barabasi-${n}-m${m}-${i}.txt"
    python generator.py barabasi $n $m > "$filename"
  done
done

# ---------------- WATTS ----------------
for n in "${sizes[@]}"
do
  for ((i=1;i<=qtd;i++))
  do
    k=4
    p=0.2
    filename="graphs/watts-${n}-k${k}-p${p}-${i}.txt"
    python generator.py watts $n $k $p > "$filename"
  done
done

echo "✔️ Grafos gerados na pasta ./graphs"