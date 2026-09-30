# Padrao Observador

`Jogador` esta acoplado diretamente a classe concreta `PainelDeConquistas`,
que ele mesmo instancia internamente. Isso impede adicionar novos tipos de
observadores (um placar na tela, um logger de eventos) sem alterar
`Jogador`, e nao permite registrar/remover observadores dinamicamente.

Refatore usando o padrao **Observador (Observer)** para desacoplar o
"sujeito" (Jogador) dos seus observadores. Siga os TODOs numerados no
arquivo `Main.java`.
