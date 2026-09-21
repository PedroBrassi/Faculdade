# Padrao Factory Method

O codigo abaixo funciona corretamente, mas tem um problema de projeto: 
- a criacao de `TCPChannel` esta duplicada em tres metodos (`f`, `g`, `h`).

Se amanha o sistema precisar passar a usar `UDPChannel`, seria necessario
alterar codigo em varios lugares diferentes.

Refatore o codigo aplicando o padrao de projeto **Factory Method**, seguindo
os TODOs numerados no arquivo `Main.java`.