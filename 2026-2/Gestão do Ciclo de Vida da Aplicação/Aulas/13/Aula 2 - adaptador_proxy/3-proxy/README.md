# Padrao Proxy (extensao: cache)

Suponha que nosso servico de pesquisa de livros esteja ficando popular e
ganhando usuarios. Para melhorar seu desempenho, pensamos em introduzir um
sistema de cache: antes de pesquisar por um livro, iremos verificar se ele
esta no cache; se sim, o livro sera imediatamente retornado; caso contrario,
a pesquisa prosseguira segundo a logica normal do metodo `getBook()`.

Porem, nao gostariamos que esse novo requisito -- pesquisa em cache -- fosse
implementado na classe `BookSearch`. O motivo e que queremos manter a classe
coesa e aderente ao Principio da Responsabilidade Unica: o cache sera
implementado por um desenvolvedor diferente, possivelmente usando uma
biblioteca de cache de terceiros. Por isso, separe em uma classe distinta o
interesse "pesquisar livros por ISBN" (requisito funcional) do interesse
"usar cache nas pesquisas" (requisito nao-funcional).

Siga os TODOs numerados no arquivo `Main.java`.
