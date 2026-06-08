#!/bin/bash

read -p "Quantas execuções (runs)? " N

if [ -z "$N" ]; then
    echo "Número inválido"
    exit 1
fi

for i in $(seq 1 $N)
do
    echo ""
    echo "=============================="
    echo "RUN $i / $N"
    echo "=============================="

    mkdir -p Results/run_$i

    ./bin/main

    cp -r Results/TodosResultados.txt Results/run_$i/
    cp -r Results/*.txt Results/run_$i/ 2>/dev/null
    cp -r Results/graficos Results/run_$i/ 2>/dev/null

    echo "Run $i finalizado"
done

echo ""
echo "Todos os runs foram concluídos."