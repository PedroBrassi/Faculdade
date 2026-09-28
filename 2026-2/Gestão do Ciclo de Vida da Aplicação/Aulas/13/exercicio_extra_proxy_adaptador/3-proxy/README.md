# Padrao Proxy (extensao: cache)

Suponha que nosso jogo carregue assets (texturas, modelos 3D, audios) sob
demanda, e que esse carregamento esteja ficando um gargalo de desempenho.
Para melhorar isso, pensamos em introduzir um sistema de cache: antes de
carregar um asset, iremos verificar se ele ja esta no cache; se sim, o
asset sera imediatamente retornado; caso contrario, o carregamento
prosseguira segundo a logica normal do metodo `getAsset()`.

Porem, nao gostariamos que esse novo requisito -- cache de assets -- fosse
implementado na classe `AssetLoader`. O motivo e que queremos manter a
classe coesa e aderente ao Principio da Responsabilidade Unica: o cache
sera implementado por um desenvolvedor diferente, possivelmente usando uma
biblioteca de cache de terceiros. Por isso, separe em uma classe distinta o
interesse "carregar assets por ID" (requisito funcional) do interesse "usar
cache nos carregamentos" (requisito nao-funcional).

Siga os TODOs numerados no arquivo `Main.java`.
