#!/bin/bash

if [ ! -f "./bin/main" ]; then
    echo "ERRO: bin/main nao encontrado. Execute primeiro: make"
    exit 1
fi

read -p "Quantas execucoes (runs)? " N

if [ -z "$N" ] || ! [[ "$N" =~ ^[0-9]+$ ]]; then
    echo "Numero invalido"
    exit 1
fi

for i in $(seq 1 $N)
do
    echo ""
    echo "=============================="
    echo "RUN $i / $N"
    echo "=============================="

    mkdir -p Results/run_$i

    ./bin/main Results/run_$i

    echo "Run $i finalizado -> Results/run_$i"
done

echo ""
echo "Todos os $N runs concluidos."
