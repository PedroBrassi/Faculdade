# Padrao Visitor

A operacao de "descrever" esta implementada diretamente dentro de cada
subclasse de `ItemDoInventario`. O problema aparece quando voce precisa
adicionar uma nova operacao sobre essa hierarquia (por exemplo, calcular o
valor de revenda de cada item): seria necessario alterar novamente
`ItemDoInventario` e todas as suas subclasses.

Refatore usando o padrao **Visitor** para permitir adicionar novas
operacoes sobre a hierarquia de `ItemDoInventario` sem modificar essas
classes. Siga os TODOs numerados no arquivo `Main.java`.
