# Padrao Template Method

O metodo `calcSalarioLiquido()` -- o "esqueleto" do algoritmo de calculo do
salario liquido -- esta duplicado em `FuncionarioCLT` e `FuncionarioPJ`;
apenas os tres metodos de calculo de desconto variam entre elas.

Refatore usando o padrao **Template Method** para eliminar essa duplicacao,
definindo o esqueleto do algoritmo uma unica vez. Siga os TODOs numerados no
arquivo `Main.java`.

*Observacao:* o exemplo original do livro trazia apenas `FuncionarioCLT`;
`FuncionarioPJ` foi adicionada a este exercicio para tornar visivel o
problema da duplicacao que o padrao resolve.
