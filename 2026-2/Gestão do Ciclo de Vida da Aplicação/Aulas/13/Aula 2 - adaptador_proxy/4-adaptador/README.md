# Padrao Adaptador

`SistemaControleProjetores` conhece diretamente as classes concretas
`ProjetorSamsung` e `ProjetorLG` (que sao bibliotecas de terceiros e nao
podem ser alteradas). Isso obriga a existencia de um metodo `init()`
sobrecarregado para cada marca, e qualquer nova marca exigiria alterar essa
classe novamente -- violando o principio Aberto/Fechado.

Refatore usando o padrao **Adaptador** para que o sistema dependa de uma
unica abstracao, sem conhecer as classes concretas dos projetores. Siga os
TODOs numerados no arquivo `Main.java`.
