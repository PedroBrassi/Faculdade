# Padrao Adaptador

`GerenciadorDeControles` conhece diretamente as classes concretas
`ControleXboxAPI` e `ControleDualSenseAPI` (que sao bibliotecas de terceiros
dos fabricantes e nao podem ser alteradas). Isso obriga a existencia de um
metodo `iniciar()` sobrecarregado para cada marca, e qualquer novo controle
que chegue ao mercado exigiria alterar essa classe novamente -- violando o
principio Aberto/Fechado.

Refatore usando o padrao **Adaptador** para que o sistema dependa de uma
unica abstracao, sem conhecer as classes concretas dos controles. Siga os
TODOs numerados no arquivo `Main.java`.
