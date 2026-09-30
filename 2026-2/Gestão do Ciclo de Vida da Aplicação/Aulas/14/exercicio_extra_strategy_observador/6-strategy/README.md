# Padrao Strategy

`Personagem.mover()` concentra a implementacao de todas as formas de
controle existentes (joystick, sensor de movimento, comando de voz) num
unico metodo, selecionando qual usar atraves de uma comparacao de String.
Isso viola o principio Aberto/Fechado (adicionar um novo tipo de controle
exige alterar `Personagem`) e mistura responsabilidades.

Refatore usando o padrao **Strategy** para que cada forma de controle seja
uma classe independente, plugavel em `Personagem` sem modifica-la. Siga os
TODOs numerados no arquivo `Main.java`.
