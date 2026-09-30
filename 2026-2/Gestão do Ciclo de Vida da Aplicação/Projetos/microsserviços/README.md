# RetroGames v1 básico

Este é o primeiro laboratório de microsserviços. O foco é reconhecer **dois processos**:
Catálogo na porta 8081 e Pedidos na porta 8082. A interface MVC está em
Pedidos. Para listar ou localizar um jogo, Pedidos faz uma chamada HTTP ao
Catálogo pelo nome `catalogo` da rede do Docker.

## Executar

Entre nesta pasta e rode:

```bash
docker compose up --build -d
docker compose ps
```

Abra http://localhost:8082, faça um pedido e observe a lista. O serviço do
Catálogo também pode ser visto em http://localhost:8081/jogos.

## Onde olhar nesta prática

1. `docker-compose.yml`: dois contêineres, duas portas e o endereço interno `CATALOGO_URL`.
2. `pedidos/CatalogoClient.cs`: requisição HTTP para o outro serviço.
3. `catalogo/Program.cs`: o serviço que responde a GET `/jogos`.
4. `pedidos/PedidoRepository.cs`: dados guardados apenas no processo de Pedidos.

A página foi construída com MVC, estudado na prática anterior. Aqui as perguntas
tratam da **fronteira entre os processos**; a estrutura interna da página não
precisa ser analisada de novo.

O Catálogo usa dados fixos e Pedidos guarda pedidos **somente na memória**.
Ao reiniciar o contêiner de Pedidos, a lista é perdida. Não há estoque,
banco, transação, RabbitMQ ou falhas simuladas nesta etapa; esses assuntos
entram nos laboratórios seguintes.

## Experimento da fronteira

Depois de fazer um pedido:

```bash
docker compose stop catalogo
```

Atualize a página. Os pedidos locais continuam visíveis, mas a consulta aos
jogos falha. Restaure o serviço com:

```bash
docker compose start catalogo
```

Pare tudo com `docker compose down` ao concluir. Execute uma versão do
RetroGames por vez, pois elas reutilizam as mesmas portas.
