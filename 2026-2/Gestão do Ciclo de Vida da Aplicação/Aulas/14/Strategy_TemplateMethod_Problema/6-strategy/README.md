# Padrao Strategy

`MyList.sort()` concentra a implementacao de todos os algoritmos de
ordenacao num unico metodo, selecionando qual usar atraves de uma
comparacao de String. Isso viola o principio Aberto/Fechado (adicionar um
novo algoritmo exige alterar `MyList`) e mistura responsabilidades.

Refatore usando o padrao **Strategy** para que cada algoritmo de ordenacao
seja uma classe independente, plugavel em `MyList` sem modifica-la. Siga os
TODOs numerados no arquivo `Main.java`.
