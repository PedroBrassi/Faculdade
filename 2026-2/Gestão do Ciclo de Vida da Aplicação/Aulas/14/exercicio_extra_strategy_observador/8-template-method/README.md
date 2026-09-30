# Padrao Template Method

O metodo `calcDanoFinal()` -- o "esqueleto" do algoritmo de calculo do dano
final de um ataque -- esta duplicado em `PersonagemGuerreiro` e
`PersonagemMago`; apenas os tres metodos de calculo de reducao de dano
variam entre eles.

Refatore usando o padrao **Template Method** para eliminar essa duplicacao,
definindo o esqueleto do algoritmo uma unica vez. Siga os TODOs numerados
no arquivo `Main.java`.
