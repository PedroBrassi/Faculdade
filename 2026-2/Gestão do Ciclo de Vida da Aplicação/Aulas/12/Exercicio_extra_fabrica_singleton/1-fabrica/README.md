# Padrao Factory Method

O codigo abaixo funciona corretamente, mas tem um problema de projeto: a
criacao de `GoblinInimigo` esta duplicada em tres pontos de spawn do jogo
(`ondaTutorial`, `ondaFase1`, `arenaFinal`). Se amanha o game design pedir
para a fase 2 passar a spawnar `OrcInimigo` em vez de `GoblinInimigo`, seria
necessario alterar codigo em varios lugares diferentes.

Refatore o codigo aplicando o padrao de projeto **Factory Method**, seguindo
os TODOs numerados no arquivo `Main.java`.
